#!/usr/bin/env python3

'''
Camera calibration program
オーム社「3D計測の基礎」掲載プログラム
  著作者：坂本 静生

参考にしたプログラム
https://docs.opencv.org/master/dc/dbb/tutorial_py_calibration.html

2021年10月10日 第1.00版リリース
'''

import numpy as np
import cv2
import glob
from matplotlib import pyplot as plt

# 終端条件の設定
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# 8x7チェッカーパターン座標位置の設定
objp = np.zeros((8*7,3), np.float32)
objp[:,:2] = np.mgrid[0:8, 0:7].T.reshape(-1,2) * 20.0

# 全ての画像に対するチェッカーパターン
#   座標位置(3D / 2D)記録用配列
objpoints = []
imgpoints = []
imgs = []

# 幾何学的キャリブレーションの
#   ターミナル出力
def printResult(ret, mtx, dist):
    print("*** キャリブレーション結果 ***")
    print("画像上への再投影RMS = ", ret, " (画素)")
    print("カメラ内部パラメータ = \n", mtx)
    print("レンズ歪 = \n", dist)

# 内部パラメータのファイル保存
def saveIntrinsicParams(mtx, dist):
    f = open("intrinsicParams.dat", "wb")
    for i in range(3):
        for j in range(3):
            f.write(mtx[i][j])
    for i in range(5):
        f.write(dist[0][i])
    f.close()

# 外部パラメータのターミナル出力
def printVecs(rvec, tvec):
    R, _ = cv2.Rodrigues(rvec)
    print("回転行列 = \n", R)
    print("回転ベクトル = \n", rvec)
    print("移動ベクトル = \n", tvec)

# メイン関数
if __name__ == '__main__':
    images = sorted(glob.glob('*.png'))
    if len(images) == 0:
        print("エラー - 画像ファイルがありません")
        exit(-1)
    elif len(images) < 10:
        print("エラー - 十分な数の画像ファイルがありません")
        exit(-1)
        
    # 画像ファイルをひとつずつ処理
    for fname in images:
        img = cv2.imread(fname)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # チェッカーパターン
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
