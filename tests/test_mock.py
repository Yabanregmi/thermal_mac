import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import pytest
import numpy as np
import cv2
from mocks.mock_camera import MockCameraController

@pytest.fixture
def mock_camera():
    cam = MockCameraController()
    yield cam
    cam.shutdown()


def test_initialization(mock_camera):
    # Ensure the VideoCapture object is created
    assert isinstance(mock_camera.cap, cv2.VideoCapture)
    assert mock_camera.trigger_next_anomaly is False


def test_get_frame_returns_frame_and_temp(mock_camera):
    frame, temp = mock_camera.get_frame()
    # Frame should be either None or a numpy array of shape (120, 160, 3)
    if frame is not None:
        assert isinstance(frame, np.ndarray)
        assert frame.shape == (120, 160, 3)
        assert 20.0 <= temp <= 65.0


def test_trigger_anomaly(mock_camera):
    # Trigger anomaly and check that the next frame has high temperature
    mock_camera.trigger_anomaly()
    frame, temp = mock_camera.get_frame()
    if frame is not None:
        assert temp >= 55.0  # Should be in anomaly range
        assert temp <= 65.0
    # Ensure trigger is reset
    assert mock_camera.trigger_next_anomaly is False


def test_multiple_get_frame(mock_camera):
    # Calling get_frame multiple times should return valid frames
    for _ in range(5):
        frame, temp = mock_camera.get_frame()
        if frame is not None:
            assert frame.shape == (120, 160, 3)
            assert 20.0 <= temp <= 65.0


def test_shutdown(mock_camera):
    # Check that shutdown releases the camera
    mock_camera.shutdown()
    # After release, cap.isOpened() should be False
    assert not mock_camera.cap.isOpened()
