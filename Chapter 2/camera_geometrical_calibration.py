#!/usr/bin/env python3

'''
Calibrate a camera geometrically, by the 8x7 checker patttern.
written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
to be released by Ohmsha, Ltd.

- version 1.00 on 1st, April, 2022.
  developed on Mouse Computer H5-CML, Ubuntu 20.04LTS.

Reference: https://docs.opencv.org/master/dc/dbb/tutorial_py_calibration.html
'''

import numpy as np
import cv2
import glob
from matplotlib import pyplot as plt

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((8*7,3), np.float32)
objp[:,:2] = np.mgrid[0:8, 0:7].T.reshape(-1,2) * 20.0

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.
# Array to store images with image points
imgs = []

def printResult(ret, mtx, dist):
    print("*** calibration result ***")
    print("RMS of reprojection = ", ret, " (pixels)")
    print("Camera matrix = \n", mtx)
    print("Lens distortion = \n", dist)

def saveIntrinsicParams(mtx, dist):
    f = open("intrinsicParams.dat", "wb")
    for i in range(3):
        for j in range(3):
            f.write(mtx[i][j])
    for i in range(5):
        f.write(dist[0][i])
    f.close()

def printVecs(rvec, tvec):
    R, _ = cv2.Rodrigues(rvec)
    print("roation matrix     = \n", R)
    print("translation vector = \n", tvec)

if __name__ == '__main__':
    images = sorted(glob.glob('*.png'))
    if len(images) == 0:
        print("error - no image file")
        exit(-1)
    elif len(images) < 10:
        print("error - not enough number of image files")
        exit(-1)
        
    for fname in images:
        img = cv2.imread(fname)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        ret, corners = cv2.findChessboardCorners(gray, (8,7), None)
        if ret:
            objpoints.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (11,11), (-1,-1), criteria)
            imgpoints.append(corners)
        else:
            print("failure to detect corners: ", fname)

        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.drawChessboardCorners(img, (8,7), corners2, ret)
        imgs.append(img)

    ret, mtx, dist, rvecs ,tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)
    printResult(ret, mtx, dist)
    if ret > 1:
        print("failure to calculate matrix")
        exit(-1)
    h, w = img.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist.ravel(), (w,h), 1, (w,h))
    print("New camera matrix = \n", newcameramtx)
    print("save camera parameters (new intrinsic matrix and distortion vector)")
    saveIntrinsicParams(newcameramtx, dist)

    i = 0
    for fname in images:
        print("\n*** image file name = ", fname, " ***")
        printVecs(rvecs[i], tvecs[i])
        img = cv2.imread(fname)
        dst = cv2.undistort(img, mtx, dist, None, newcameramtx)
        dst = cv2.cvtColor(dst, cv2.COLOR_BGR2RGB)
        fig = plt.figure(figsize=(14,6))
        fig.canvas.manager.set_window_title(fname)
        out = cv2.hconcat([imgs[i], dst])
        plt.imshow(out)
        plt.axis('off')
        plt.show()
        i = i + 1

# end of program.
