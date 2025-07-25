# import sqlite3

# conn = sqlite3.connect("data.db")
# cursor = conn.cursor()

# cursor.execute("PRAGMA table_info(latest_data);")
# columns = cursor.fetchall()

# for col in columns:
#     print(col)

import sqlite3
import numpy as np
import cv2

# Step 1: Connect to DB
conn = sqlite3.connect("data.db")
cursor = conn.cursor()

# Step 2: Fetch all image blobs ordered by slot_id
cursor.execute("SELECT image FROM latest_data ORDER BY slot_id;")
rows = cursor.fetchall()

if not rows:
    raise ValueError("No image data found in database!")

# Step 3: Decode first image to get size
first_img_blob = rows[0][0]
first_array = np.frombuffer(first_img_blob, dtype=np.uint8)
first_img = cv2.imdecode(first_array, cv2.IMREAD_COLOR)

if first_img is None:
    raise ValueError("Failed to decode the first image.")

height, width, _ = first_img.shape

# Step 4: Initialize VideoWriter
fourcc = cv2.VideoWriter_fourcc(*'XVID')
fps = 10.0
out = cv2.VideoWriter("output_from_db.avi", fourcc, fps, (width, height))

# Step 5: Loop through all images and write to video
for i, row in enumerate(rows):
    blob = row[0]
    img_array = np.frombuffer(blob, dtype=np.uint8)
    img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)

    if img is None:
        print(f"Warning: Failed to decode image at index {i}, skipping.")
        continue

    out.write(img)

# Step 6: Finalize
out.release()
conn.close()
print("Video saved as output_from_db.avi")
