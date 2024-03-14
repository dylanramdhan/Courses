#Team6-Xiong-Ramdhan-Gumusayak-Ramondo - 12/1   
import time

from picamera2 import Picamera2, Preview

picam2 = Picamera2()


picam2.start()
while(1):
    time.sleep(0.1)
    metadata = picam2.capture_file("test.jpg")
    print(metadata)

picam2.close()
