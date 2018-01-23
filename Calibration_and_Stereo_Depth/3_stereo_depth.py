import sys
import numpy as np
import cv2

REMAP_INTERPOLATION = cv2.INTER_LINEAR

DEPTH_VISUALIZATION_SCALE = 2048

calibration = np.load("capture/calibration.npz", allow_pickle=False)
imageSize = tuple(calibration["imageSize"])
leftMapX = calibration["leftMapX"]
leftMapY = calibration["leftMapY"]
leftROI = tuple(calibration["leftROI"])
rightMapX = calibration["rightMapX"]
rightMapY = calibration["rightMapY"]
rightROI = tuple(calibration["rightROI"])

#set fix camera resolution
CAMERA_WIDTH = 1280
CAMERA_HEIGHT = 960

#pull left and right video capture 
leftCamera = cv2.VideoCapture(1)
rightCamera = cv2.VideoCapture(2)

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

#disparity settings
window_size = 5
min_disp = 32
num_disp = 112-min_disp
stereoMatcher = cv2.StereoSGBM_create(
    minDisparity = min_disp,
    numDisparities = num_disp,
    uniquenessRatio = 10,
    speckleWindowSize = 100,
    speckleRange = 32,
    disp12MaxDiff = 1,
    P1 = 8*3*window_size**2,
    P2 = 32*3*window_size**2,
)

#Grab both frames first, then retrieve to minimize latency between cameras
#bring the calibration into the Livepicture, colored this picture gray
#save and overwrite the stereo depth picture for later analysis
while(True):

    if not leftCamera.grab() or not rightCamera.grab():
        print("No more frames")
        break

    _, leftFrame = leftCamera.retrieve()
    leftFrame = cropHorizontal(leftFrame)
    leftHeight, leftWidth = leftFrame.shape[:2]
    _, rightFrame = rightCamera.retrieve()
    rightFrame = cropHorizontal(rightFrame)
    rightHeight, rightWidth = rightFrame.shape[:2]

    if (leftWidth, leftHeight) != imageSize:
        print("Left camera has different size than the calibration data")
        break

    if (rightWidth, rightHeight) != imageSize:
        print("Right camera has different size than the calibration data")
        break

    fixedLeft = cv2.remap(leftFrame, leftMapX, leftMapY, REMAP_INTERPOLATION)
    fixedRight = cv2.remap(rightFrame, rightMapX, rightMapY, REMAP_INTERPOLATION)

    grayLeft = cv2.cvtColor(fixedLeft, cv2.COLOR_BGR2GRAY)
    grayRight = cv2.cvtColor(fixedRight, cv2.COLOR_BGR2GRAY)
    disparity = stereoMatcher.compute(grayLeft, grayRight).astype(np.float32) / 16.0
    disparity = (disparity-min_disp)/num_disp
    picture = disparity
    picture = picture*255
    cv2.imwrite("capture/000000.jpg", picture)
    
    cv2.imshow('left', fixedLeft)
    cv2.imshow('right', fixedRight)
    cv2.imshow('depth', disparity)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    
leftCamera.release()
rightCamera.release()
cv2.destroyAllWindows()