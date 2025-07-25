#! /usr/bin/env python3
from ctypes.util import find_library
import requests
import numpy as np
import ctypes as ct
import cv2
import os
import datetime

#Define EvoIRFrameMetadata structure for additional frame infos
class EvoIRFrameMetadata(ct.Structure):
     _fields_ = [("counter", ct.c_uint),
                 ("counterHW", ct.c_uint),
                 ("timestamp", ct.c_longlong),
                 ("timestampMedia", ct.c_longlong),
                 ("flagState", ct.c_int),
                 ("tempChip", ct.c_float),
                 ("tempFlag", ct.c_float),
                 ("tempBox", ct.c_float),
                 ]

# load library
pathXml = '/home/dev-adm/Documents/Workspace/projectX/config/24104186.xml'
if os.name == 'nt':
        #windows:
        libir = ct.CDLL('.\\libirimager.dll')
        pathXml = b'.\generic.xml'
else:
        #linux:
        libir = ct.cdll.LoadLibrary(ct.util.find_library("irdirectsdk"))
        pathXml = b'/home/dev-adm/Documents/Workspace/projectX/config/24104186.xml'

# init vars
pathFormat, pathLog = b'', b''
palette_width, palette_height = ct.c_int(), ct.c_int()
thermal_width, thermal_height = ct.c_int(), ct.c_int()
serial = ct.c_ulong()

# init EvoIRFrameMetadata structure
metadata = EvoIRFrameMetadata()

# init lib
ret = libir.evo_irimager_usb_init(pathXml, pathFormat, pathLog)

# get the serial number
ret = libir.evo_irimager_get_serial(ct.byref(serial))
print('serial: ' + str(serial.value))

# get thermal image size
libir.evo_irimager_get_thermal_image_size(ct.byref(thermal_width), ct.byref(thermal_height))
print('thermal width: ' + str(thermal_width.value))
print('thermal height: ' + str(thermal_height.value))

# init thermal data container
np_thermal = np.zeros([thermal_width.value * thermal_height.value], dtype=np.uint16)
npThermalPointer = np_thermal.ctypes.data_as(ct.POINTER(ct.c_ushort))

# get palette image size, width is different to thermal image width due to stride alignment!!!
libir.evo_irimager_get_palette_image_size(ct.byref(palette_width), ct.byref(palette_height))
print('palette width: ' + str(palette_width.value))
print('palette height: ' + str(palette_height.value))

# init image container
np_img = np.zeros([palette_width.value * palette_height.value * 3], dtype=np.uint8)
npImagePointer = np_img.ctypes.data_as(ct.POINTER(ct.c_ubyte))

# get timestamp for the image. metadata.timestamp() will show faulty values under windows
# as it uses directShow() which is now outdated. Still works fine with Linux based OS.
# Alternative, CounterHW can be used to get the HW Counter from the camera directly.
# (n)HZ = 1Sec -> (n) CounterHW = 1Sec
time_stamp = datetime.datetime.now().strftime("%H:%M:%S %d %B %Y")
show_time_stamp = False

# Skalierungsfaktor definieren
scale_factor = 6  # z.B. 4-fache Vergrößerung
# Alternativ: Zielauflösung angeben (falls feste Abmessungen gewünscht)
target_width = palette_width.value * scale_factor
target_height = palette_height.value * scale_factor

# URL des Express-Servers
server_url = 'http://localhost:3000/rxIRData'

try:
    # capture and display image till q is pressed
    while chr(cv2.waitKey(1) & 255) != 'q':

            if show_time_stamp:
                print(time_stamp)

            #get thermal and palette image with metadat
            ret = libir.evo_irimager_get_thermal_palette_image_metadata(thermal_width, thermal_height, npThermalPointer, palette_width, palette_height, npImagePointer, ct.byref(metadata))

            if ret != 0:
                    print('error on evo_irimager_get_thermal_palette_image ' + str(ret))
                    continue

            #calculate max value
            mean_temp = np_thermal.max() / 10. - 100

            #mean_temp = mean_temp / 10. - 100
            #print('max temp: ' + str(mean_temp))
            original_image = np_img.reshape(palette_height.value, palette_width.value, 3)[:, :, ::-1]
            resized_image = cv2.resize(original_image, (target_width, target_height), interpolation=cv2.INTER_LINEAR)

            # Hochskaliertes Bild anzeigen
            #cv2.imshow('image', resized_image)


            # Bild in JPEG kodieren
            _, buffer = cv2.imencode('.jpg', original_image)

            temperature = mean_temp  # Beispiel: Temperatur zwischen 20°C und 30°C

            # Sende das Bild und die Temperaturdaten an den Server
            headers = {
                'Content-Type': 'application/octet-stream',
                'X-Temperature': str(temperature)  # Temperatur als Header senden
            }

            try:
                response = requests.post(server_url, data=buffer.tobytes(), headers=headers)
                # print(f'Bild und Temperatur gesendet. Status: {response.status_code}')
            except Exception as e:
                print('Fehler beim Senden der Daten:', e)
finally:
    # clean shutdown
    libir.evo_irimager_terminate()