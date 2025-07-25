import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))


import pytest
import json
import os
import cv2
import numpy as np
import datetime
from pathlib import Path
from unittest.mock import patch, MagicMock

import main    

@pytest.fixture
def mock_frame():
    """Generate a mock frame (160x120 image)."""
    return np.zeros((120, 160, 3), dtype=np.uint8)


# ---------- Config Tests ----------

def test_load_config(tmp_path):
    config_file = tmp_path / "config.json"
    config_data = {
        "start_threshold": 60,
        "stop_threshold": 55,
        "manual_record_limit": 400,
        "save_dir": str(tmp_path),
        "duration": 10
    }
    config_file.write_text(json.dumps(config_data))

    with patch("main.CONFIG_FILE", config_file):
        main.load_config()
        assert main.START_THRESHOLD == 60
        assert main.STOP_THRESHOLD == 55
        assert main.MANUAL_RECORD_LIMIT == 400
        assert main.POST_EVENT_DURATION == 10
        assert main.save_dir == tmp_path




def test_save_config(tmp_path):
    with patch("main.CONFIG_FILE", tmp_path / "config.json"):
        main.START_THRESHOLD = 55
        main.STOP_THRESHOLD = 45
        main.save_dir = tmp_path
        main.POST_EVENT_DURATION = 7
        main.save_config()

        data = json.loads((tmp_path / "config.json").read_text())
        assert data["start_threshold"] == 55
        assert data["stop_threshold"] == 45
        assert data["duration"] == 7       


def test_set_threshold():
    with patch("main.save_config") as mock_save:
        main.set_threshold(70)
        assert main.TEMP_THRESHOLD == 70
        mock_save.assert_called_once()


def test_set_duration():
    with patch("main.save_config") as mock_save:
        main.set_duration(120)
        assert main.POST_EVENT_DURATION == 120
        main.set_duration(999)  # Check max limit
        assert main.POST_EVENT_DURATION <= 180
        mock_save.assert_called()


def test_set_save_dir(tmp_path):
    path_str = tmp_path / "output"
    with patch("main.save_config") as mock_save:
        main.set_save_dir(path_str)
        assert main.save_dir == path_str
        mock_save.assert_called_once()


# ---------- Core Functions ----------

def test_generate_error_image():
    img = main.generate_error_image(100, 50)
    assert img.shape == (50, 100, 3)
    assert isinstance(img, np.ndarray)


def test_screenshot(tmp_path, mock_frame):
    main.save_dir = tmp_path
    main.screenshot(mock_frame)
    files = list(tmp_path.glob("screenshot_*.png"))
    assert len(files) > 0


def test_save_frames_as_video(tmp_path, mock_frame):
    filename = tmp_path / "test_video.avi"
    main.save_frames_as_video([mock_frame] * 5, filename, fps=10)
    assert filename.exists()


def test_record_video(mock_frame):
    mock_cam = MagicMock()
    mock_cam.get_frame.return_value = (mock_frame, 40.0)
    with patch("cv2.VideoWriter") as mock_writer:
        instance = mock_writer.return_value
        instance.isOpened.return_value = True
        main.record_video(mock_cam, main.SystemMode.NORMAL, duration=1)
        assert instance.write.called


def test_save_anomaly_video(tmp_path, mock_frame):
    mock_cam = MagicMock()
    mock_cam.get_frame.return_value = (mock_frame, 40.0)
    with patch.object(main, "FrameDatabase") as mock_db:
        mock_db.return_value.get_frames_from_last_n_seconds.return_value = [mock_frame]
        main.save_dir = tmp_path
        main.save_anomaly_video(mock_cam, "fake.db", 55.0, "timestamp", tmp_path, duration=1)
        files = list(tmp_path.glob("merged_anomaly_*.avi"))
        assert len(files) > 0


def test_display(mock_frame):
    result = main.display(mock_frame, 60.0, main.SystemMode.NORMAL, True)
    assert result.shape == mock_frame.shape
    assert isinstance(result, np.ndarray)


# ---------- Backend Callable Functions ----------

def test_set_mode():
    assert main.set_mode(main.SystemMode.TEST) is True
    assert main.mode == main.SystemMode.TEST
    assert main.set_mode("Invalid") is False


def test_get_system_status():
    status = main.get_system_status()
    assert "mode" in status
    assert "threshold" in status
    assert "recording" in status


def test_start_manual_recording_from_server(mock_frame):
    mock_cam = MagicMock()
    mock_cam.get_frame.return_value = (mock_frame, 30.0)
    main.cam = mock_cam
    with patch("threading.Thread") as mock_thread:
        main.recording = False
        result = main.start_manual_recording_from_server()
        assert result is True
        assert mock_thread.called


def test_trigger_mock_anomaly():
    main.USE_MOCK_CAMERA = True
    main.cam = MagicMock()
    assert main.trigger_mock_anomaly() is True


def test_trigger_hupe(caplog):
    caplog.set_level("INFO")
    main.trigger_hupe()
    assert "HUPE TRIGGERED" in caplog.text

def test_trigger_blitz(caplog):
    caplog.set_level("INFO")
    main.trigger_blitz()
    assert "BLITZ TRIGGERED" in caplog.text



def test_relais_functions():
    main.relais_frozen = False
    main.set_relais_state(True)
    main.freeze_relais()
    assert main.relais_frozen is True
    main.unfreeze_relais()
    assert main.relais_frozen is False


def test_take_screenshot_from_server(tmp_path, mock_frame):
    main.save_dir = tmp_path
    main.frame = mock_frame
    assert main.take_screenshot_from_server() is True


# ---------- Main Loop ----------

def test_main_loop_quit(monkeypatch):
    """Test main loop exit on 'q' key press."""
    monkeypatch.setattr("cv2.waitKey", lambda x: ord('q'))
    main.exit_flag = False
    main.cam = MagicMock()
    main.cam.get_frame.return_value = (np.zeros((120, 160, 3)), 30.0)
    with patch.object(main, "FrameDatabase") as mock_db:
        mock_db.return_value.insert_frame = MagicMock()
        main.main()
        assert main.exit_flag is True

