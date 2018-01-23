import numpy as np
import cv2
import glob
import os

#Clear's picture folder befor capturing
filesLeft = glob.glob('capture/left/*')
for f in filesLeft:
    os.remove(f)
    
filesRight = glob.glob('capture/right/*')
for f in filesRight:
    os.remove(f)

#set save path for both cameras
LEFT_PATH = "capture/left/{:06d}.jpg"
RIGHT_PATH = "capture/right/{:06d}.jpg"

#pull left and right video capture 
leftCamera = cv2.VideoCapture(1)
rightCamera = cv2.VideoCapture(2)

#set fix camera resolution
CAMERA_WIDTH = 1280
CAMERA_HEIGHT = 960

#Increase the resolution
leftCamera.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
leftCamera.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
rightCamera.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
rightCamera.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)

#Use MJPEG to avoid overloading the USB 2.0 bus at this resolution
leftCamera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))
rightCamera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))

#discard the edges for a good calibration because of the distortion in the left an right edges
CROP_WIDTH = 1000
def cropHorizontal(image):
    return image[:,
            int((CAMERA_WIDTH-CROP_WIDTH)/2):
            int(CROP_WIDTH+(CAMERA_WIDTH-CROP_WIDTH)/2)]

#Frame counter
frameId = 0

#Grab both frames first, then retrieve to minimize latency between cameras
#and save every third picture in the declared path as long the program is running
while(True):
    if not (leftCamera.grab() and rightCamera.grab()):
        print("No more frames")
        break

    _, leftFrame = leftCamera.retrieve()
    leftFrame = cropHorizontal(leftFrame)
    _, rightFrame = rightCamera.retrieve()
    rightFrame = cropHorizontal(rightFrame)
    
    if(frameId % 3 == 0):
        cv2.imwrite(LEFT_PATH.format(frameId), leftFrame)
        cv2.imwrite(RIGHT_PATH.format(frameId), rightFrame)

    cv2.imshow('left', leftFrame)
    cv2.imshow('right', rightFrame)
    #pause the program if q is hold
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    frameId += 1

leftCamera.release()
rightCamera.release()
cv2.destroyAllWindows()