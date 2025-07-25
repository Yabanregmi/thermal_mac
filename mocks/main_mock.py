import cv2
import numpy as np
import datetime
from pathlib import Path
import time
import threading
import json
import logging
import csv

from frame_database import FrameDatabase

USE_MOCK_CAMERA = True
TEMP_THRESHOLD = 50.0
POST_EVENT_DURATION = 5
CONFIG_FILE = "config.json"
LOG_FILE = "system.log"
FRAME_LOG_FILE = "frame_log.csv"

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        logging.FileHandler(LOG_FILE),
        logging.StreamHandler()
    ]
)

if not Path(FRAME_LOG_FILE).exists():
    with open(FRAME_LOG_FILE, mode='w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["timestamp", "mode", "temperature", "recording"])

if USE_MOCK_CAMERA:
    from mocks.mock_camera import MockCameraController as CameraController
else:
    from camera_control import CameraController

class SystemMode:
    NORMAL = "Normal"
    TEST = "Test"
    FAULT = "Fault"

# Global Variables 
cam = None
db = None
mode = SystemMode.NORMAL
frame = None
temp = None
recording = False
anomaly_thread = None
manual_record_thread = None
save_dir = Path("Output_data")
save_dir.mkdir(exist_ok=True)
camera_lock = threading.Lock()
db_lock = threading.Lock()
last_trigger_time = 0
last_test_time = time.time()
exit_flag = False
relais_frozen = False

# Config Load/Save 
def load_config():
    global TEMP_THRESHOLD, save_dir, POST_EVENT_DURATION
    if Path(CONFIG_FILE).exists():
        with open(CONFIG_FILE, "r") as f:
            config = json.load(f)
            TEMP_THRESHOLD = config.get("threshold", TEMP_THRESHOLD)
            save_dir = Path(config.get("save_dir", str(save_dir)))
            POST_EVENT_DURATION = config.get("duration", POST_EVENT_DURATION)
            save_dir.mkdir(exist_ok=True)
            logging.info("Config loaded.")

def save_config():
    config = {
        "threshold": TEMP_THRESHOLD,
        "save_dir": str(save_dir),
        "duration": POST_EVENT_DURATION
    }
    with open(CONFIG_FILE, "w") as f:
        json.dump(config, f)
        logging.info("Config saved.")

def set_threshold(value):
    global TEMP_THRESHOLD
    TEMP_THRESHOLD = value
    logging.info(f"Threshold set to {TEMP_THRESHOLD} by server")
    save_config()

def set_duration(seconds):
    global POST_EVENT_DURATION
    POST_EVENT_DURATION = min(max(0, seconds), 180)
    logging.info(f"Recording duration set to {POST_EVENT_DURATION} by server")
    save_config()

def set_save_dir(path_str):
    global save_dir
    save_dir = Path(path_str)
    save_dir.mkdir(exist_ok=True)
    logging.info(f"Save directory set to {save_dir} by server")
    save_config()

# Core Functions 
def generate_error_image(width=160, height=120):
    img = np.zeros((height, width, 3), dtype=np.uint8)
    cv2.putText(img, "CAMERA ERROR", (10, height // 2), cv2.FONT_HERSHEY_SIMPLEX,
                0.6, (0, 0, 255), 2)
    return img

def screenshot(frame_copy):
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = save_dir / f"screenshot_{timestamp}.png"
    cv2.imwrite(str(filename), frame_copy)
    logging.info(f"Screenshot saved as {filename}")

def save_frames_as_video(frames, filename, fps=32):
    if not frames:
        return
    height, width, _ = frames[0].shape
    out = cv2.VideoWriter(str(filename), cv2.VideoWriter_fourcc(*'MJPG'), fps, (width, height))
    for frame in frames:
        out.write(frame)
    out.release()

def record_video(cam, mode, duration=5):
    global exit_flag
    try:
        with camera_lock:
            frame, temp = cam.get_frame()
    except Exception as e:
        logging.error("Failed to start recording: %s", e)
        return
    if frame is None or frame.shape[0] == 0 or frame.shape[1] == 0:
        logging.error("Invalid frame received.")
        return

    filename = save_dir / f"thermal_video_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.avi"
    height, width = frame.shape[:2]
    writer = cv2.VideoWriter(str(filename), cv2.VideoWriter_fourcc(*'MJPG'), 32, (width, height))
    if not writer.isOpened():
        logging.error("Failed to open video writer.")
        return

    start_time = datetime.datetime.now()
    logging.info("Recording started.")

    while not exit_flag:
        with camera_lock:
            frame, temp = cam.get_frame()
        if frame is None:
            continue
        writer.write(frame)
        if (datetime.datetime.now() - start_time).total_seconds() >= duration:
            break

    writer.release()
    logging.info("Recording finished and saved.")

def save_anomaly_video(cam, db_path, temp, timestamp, save_dir, duration=5, fps=32):
    global exit_flag
    try:
        with db_lock:
            db = FrameDatabase(db_path)
            retrospective_frames = db.get_frames_from_last_n_seconds(seconds=10)
            db.close()

        post_frames = []
        start_time = time.time()
        while time.time() - start_time < duration:
            if exit_flag:
                break
            with camera_lock:
                frame, _ = cam.get_frame()
            if frame is not None:
                post_frames.append(frame)
            time.sleep(1 / fps)

        all_frames = retrospective_frames + post_frames
        filename = save_dir / f"merged_anomaly_temp{int(temp)}_{timestamp}.avi"
        save_frames_as_video(all_frames, filename, fps=fps)
        logging.info(f"Combined anomaly video saved as {filename}")
    except Exception as e:
        logging.error("Error in anomaly video thread: %s", e)

def display(frame, temp, mode, recording):
    annotated = np.ascontiguousarray(frame.copy())
    cv2.putText(annotated, f"Mode: {mode}", (10, 40),
                cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 255), 2)
    rec_status = "RECORDING" if recording else "IDLE"
    rec_color = (0, 0, 255) if recording else (0, 255, 0)
    cv2.putText(annotated, f"Recording: {rec_status}", (10, 80),
                cv2.FONT_HERSHEY_SIMPLEX, 1.0, rec_color, 2)
    if temp is not None:
        label = f"{temp:.2f}°C"
        temp_color = (0, 0, 255) if temp > TEMP_THRESHOLD else (255, 255, 0)
        cv2.putText(annotated, label, (10, 120),
                    cv2.FONT_HERSHEY_SIMPLEX, 1.0, temp_color, 2)
    else:
        cv2.putText(annotated, "Temp: N/A", (10, 120),
                    cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0, 255, 255), 2)
    return annotated

# Backend Callable Functions 
def set_mode(new_mode):
    global mode, last_test_time
    if new_mode in [SystemMode.NORMAL, SystemMode.TEST, SystemMode.FAULT]:
        mode = new_mode
        if mode == SystemMode.TEST:
            last_test_time = time.time()
        logging.info(f"Mode set to {mode} by server")
        return True
    logging.warning("Invalid mode requested: %s", new_mode)
    return False

def get_system_status():
    return {
        "mode": mode,
        "threshold": TEMP_THRESHOLD,
        "recording": recording,
        "last_trigger_time": last_trigger_time
    }

def start_manual_recording_from_server():
    global manual_record_thread, recording
    if not recording:
        manual_record_thread = threading.Thread(
            target=record_video, args=(cam, mode, POST_EVENT_DURATION)
        )
        manual_record_thread.start()
        recording = True
        logging.info("Manual recording triggered by server")
        return True
    logging.info("Manual recording already in progress")
    return False

def trigger_mock_anomaly():
    if USE_MOCK_CAMERA:
        cam.trigger_anomaly()
        logging.info("Anomaly triggered in mock camera (by server)")
        return True
    logging.warning("Trigger ignored: Not using mock camera")
    return False

def trigger_hupe():
    logging.info("HUPE TRIGGERED")

def trigger_blitz():
    logging.info("BLITZ TRIGGERED")

def set_relais_state(state):
    global relais_frozen
    if relais_frozen:
        logging.info("Attempted to change relais, but relais are frozen.")
        return
    logging.info(f"RELAIS SET TO: {'ON' if state else 'OFF'}")

def freeze_relais():
    global relais_frozen
    relais_frozen = True
    logging.info("RELAIS STATE FROZEN")

def unfreeze_relais():
    global relais_frozen
    relais_frozen = False
    logging.info("RELAIS STATE UNFROZEN")

def trigger_hupe_from_server():
    if mode == SystemMode.TEST:
        trigger_hupe()
        return True
    return False

def trigger_blitz_from_server():
    if mode == SystemMode.TEST:
        trigger_blitz()
        return True
    return False

def set_relais_state_from_server(state: bool):
    if mode == SystemMode.TEST:
        set_relais_state(state)
        return True
    return False

def freeze_relais_from_server():
    if mode == SystemMode.TEST:
        freeze_relais()
        return True
    return False

def take_screenshot_from_server():
    global frame
    if frame is not None:
        threading.Thread(target=screenshot, args=(frame.copy(),)).start()
        return True
    return False

# Main Loop 
def main():
    load_config()
    global cam, db, mode, frame, temp, recording
    global anomaly_thread, manual_record_thread
    global last_trigger_time, last_test_time, exit_flag

    RETRIGGER_COOLDOWN = 15
    TEST_TIMEOUT = 180
    last_trigger_time = 0
    last_test_time = time.time()

    try:
        cam = CameraController()
        db = FrameDatabase("frame_store.db")
    except Exception as e:
        logging.critical("Failed to initialize camera or DB: %s", e)
        mode = SystemMode.FAULT

    try:
        while True:
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                logging.info("Exiting now...")
                exit_flag = True
                break
            elif key == ord('f'):
                set_mode(SystemMode.FAULT)
            elif key == ord('t'):
                set_mode(SystemMode.TEST)
            elif key == ord('n'):
                set_mode(SystemMode.NORMAL)
            elif key == ord('s') and frame is not None:
                threading.Thread(target=screenshot, args=(frame.copy(),)).start()
            elif key == ord('v') and frame is not None and not recording:
                manual_record_thread = threading.Thread(target=record_video, args=(cam, mode, POST_EVENT_DURATION))
                manual_record_thread.start()
                recording = True
            elif key == ord('a') and USE_MOCK_CAMERA:
                cam.trigger_anomaly()
            elif key == ord('h') and mode == SystemMode.TEST:
                trigger_hupe()
            elif key == ord('b') and mode == SystemMode.TEST:
                trigger_blitz()
            elif key == ord('r') and mode == SystemMode.TEST:
                set_relais_state(True)
            elif key == ord('z') and mode == SystemMode.TEST:
                freeze_relais()
            elif key == ord('u') and mode == SystemMode.TEST:
                unfreeze_relais()

            if mode == SystemMode.TEST and (time.time() - last_test_time) > TEST_TIMEOUT:
                logging.info("Test mode timeout. Switching to NORMAL.")
                mode = SystemMode.NORMAL

            with camera_lock:
                frame, temp = cam.get_frame()

            if frame is not None:
                try:
                    with db_lock:
                        db.insert_frame(frame)
                    timestamp = datetime.datetime.now().isoformat()
                    logging.info("Frame stored at timestamp: %s", timestamp)
                    with open(FRAME_LOG_FILE, mode='a', newline='') as csvfile:
                        writer = csv.writer(csvfile)
                        writer.writerow([timestamp, mode, f"{temp:.2f}" if temp is not None else "N/A", recording])
                except Exception as e:
                    logging.warning("DB insert error: %s", e)

            current_time = time.time()
            if temp is not None and temp > TEMP_THRESHOLD:
                if current_time - last_trigger_time > RETRIGGER_COOLDOWN:
                    logging.info(f"ALERT: Anomaly detected! Temp = {temp:.2f} deg C")
                    if mode == SystemMode.NORMAL:
                        logging.info("Triggering IOs due to anomaly in NORMAL mode")
                        try:
                            trigger_blitz()
                            trigger_hupe()
                            set_relais_state(True)
                        except Exception as e:
                            logging.error("Error triggering IO in Normal mode: %s", e)
                    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
                    anomaly_thread = threading.Thread(
                        target=save_anomaly_video,
                        args=(cam, "frame_store.db", temp, timestamp, save_dir, POST_EVENT_DURATION)
                    )
                    anomaly_thread.start()
                    recording = True
                    mode = SystemMode.FAULT
                    last_trigger_time = current_time
                else:
                    logging.info("Anomaly still active, retrigger cooldown in effect (%0.2f C)", temp)

            if anomaly_thread and not anomaly_thread.is_alive():
                recording = False
                anomaly_thread = None
            if manual_record_thread and not manual_record_thread.is_alive():
                recording = False
                manual_record_thread = None

            if frame is not None:
                resized = cv2.resize(frame, (frame.shape[1]*3, frame.shape[0]*3))
                display_frame = display(resized, temp, mode, recording)
                cv2.imshow("Thermal View", display_frame)

    finally:
        if cam and hasattr(cam, "shutdown"):
            cam.shutdown()
        if db:
            db.close()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()