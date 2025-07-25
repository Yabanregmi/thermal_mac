import cv2
import numpy as np

class MockCameraController:
    def __init__(self):
        self.cap = cv2.VideoCapture(0)
        self.trigger_next_anomaly = False

    def get_frame(self):
        ret, frame = self.cap.read()
        if not ret:
            return None, None

        # Resize frame to 160x120 (mock thermal image size)
        frame = cv2.resize(frame, (160, 120))

        # Generate temperature based on trigger flag
        if self.trigger_next_anomaly:
            mean_temp = np.random.uniform(55.0, 65.0)  # Anomaly temperature
            self.trigger_next_anomaly = False          # Reset after one use
        else:
            mean_temp = np.random.uniform(20.0, 48.0)  # Normal temperature

        return frame, mean_temp

    def trigger_anomaly(self):
        self.trigger_next_anomaly = True

    def shutdown(self):
        self.cap.release()
