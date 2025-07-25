import sqlite3
import cv2
import time
import numpy as np
import logging

class FrameDatabase:
    def __init__(self, db_path="frame_store.db"):
        try:
            self.conn = sqlite3.connect(db_path, check_same_thread=False)
            self.conn.execute('''
                CREATE TABLE IF NOT EXISTS frames (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    timestamp REAL,
                    image BLOB
                )
            ''')
            self.conn.execute('CREATE INDEX IF NOT EXISTS idx_timestamp ON frames (timestamp)')
            self.conn.commit()
            logging.info(f"[DB] Connected to {db_path}")
        except Exception as e:
            logging.error(f"[DB] Failed to initialize database: {e}")
            raise

    def insert_frame(self, frame):
        try:
            timestamp = time.time()
            success, buffer = cv2.imencode('.jpg', frame)
            if success:
                self.conn.execute(
                    "INSERT INTO frames (timestamp, image) VALUES (?, ?)",
                    (timestamp, buffer.tobytes())
                )
                self.conn.commit()
                logging.debug(f"[DB] Frame inserted at {timestamp}")
            else:
                logging.warning("[DB] Frame encoding failed.")
        except Exception as e:
            logging.error(f"[DB] Error inserting frame: {e}")

    def get_frames_from_last_n_seconds(self, seconds=10):
        try:
            now = time.time()
            start_time = now - seconds
            cursor = self.conn.execute(
                "SELECT image FROM frames WHERE timestamp >= ? ORDER BY timestamp ASC",
                (start_time,)
            )
            frames = [cv2.imdecode(np.frombuffer(row[0], np.uint8), cv2.IMREAD_COLOR)
                      for row in cursor.fetchall()]
            logging.debug(f"[DB] Retrieved {len(frames)} frames from last {seconds} seconds.")
            return frames
        except Exception as e:
            logging.error(f"[DB] Error retrieving frames: {e}")
            return []

    def close(self):
        try:
            self.conn.close()
            logging.info("[DB] Connection closed.")
        except Exception as e:
            logging.error(f"[DB] Error closing database: {e}")
