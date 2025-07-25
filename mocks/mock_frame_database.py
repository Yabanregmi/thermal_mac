import sqlite3
import logging
import os

class MockFrameDatabase:
    def __init__(self, db_path="mock_frame_store.db", *args, **kwargs):
        self.db_path = db_path
        self.conn = sqlite3.connect(self.db_path, check_same_thread=False)
        self.conn.execute('''
            CREATE TABLE IF NOT EXISTS frames (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                timestamp REAL,
                image BLOB
            )
        ''')
        self.conn.commit()
        logging.info(f"[MOCK DB] Initialized mock database at {self.db_path}.")

    def insert_frame(self, frame):
        logging.info("[MOCK DB] Frame inserted (simulated).")

    def get_frames_from_last_n_seconds(self, seconds=10):
        logging.info(f"[MOCK DB] Returning empty frame list for last {seconds}s.")
        return []

    def close(self):
        self.conn.close()
        logging.info("[MOCK DB] Database closed.")
