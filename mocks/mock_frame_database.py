import logging
import time
from collections import deque

class MockFrameDatabase:
    def __init__(self, *args, buffer_seconds=10, fps=32, **kwargs):
        """
        A mock database that stores frames in memory for anomaly detection.
        buffer_seconds: how many seconds of frames to keep in memory.
        fps: expected frames per second.
        """
        self.frame_buffer = deque(maxlen=buffer_seconds * fps)
        self.timestamp_buffer = deque(maxlen=buffer_seconds * fps)
        self.fps = fps
        logging.info("[MOCK DB] Initialized in-memory frame storage.")

    def insert_frame(self, frame):
        """
        Store the frame with the current timestamp in memory.
        """
        self.frame_buffer.append(frame)
        self.timestamp_buffer.append(time.time())
        logging.info(f"[MOCK DB] Frame stored (total {len(self.frame_buffer)} frames).")

    def get_frames_from_last_n_seconds(self, seconds=10):
        """
        Return frames from the last `seconds` seconds.
        """
        current_time = time.time()
        frames = [
            f for f, ts in zip(self.frame_buffer, self.timestamp_buffer)
            if (current_time - ts) <= seconds
        ]
        logging.info(f"[MOCK DB] Returning {len(frames)} frames from last {seconds} seconds.")
        return frames

    def close(self):
        """
        Clear the buffer (simulating DB close).
        """
        logging.info("[MOCK DB] Database closed.")
        self.frame_buffer.clear()
        self.timestamp_buffer.clear()
