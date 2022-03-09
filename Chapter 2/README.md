# 第2章

2章では次の機材を用いて、正弦波格子位相シフト法に基づいて3次元計測を行うためのプログラムです。詳細は本文を参照してください。ソフトウェアはOSSのみを用いています。次は、本文で記載した、あるいは本文中のプログラムで直接呼び出すものを挙げています。必要に応じて、依存関係にあるOSSもインストールしてください。
+ ubuntu Desktop 20.04LTS: [https://jp.ubuntu.com/](https://jp.ubuntu.com/)
+ Python 3.8: [https://www.python.org/](https://www.python.org/)
+ Numpy 1.21.1: [https://numpy.org/](https://numpy.org/)
+ OpenCV 4.5: [https://opencv.org/](https://opencv.org/)
+ kivy 2.0.0: [https://kivy.org/](https://kivy.org/)
+ Matplotlib 3.4.3: [https://matplotlib.org/](https://matplotlib.org/)
+ matplotlib-cpp: [https://github.com/lava/matplotlib-cpp/](https://github.com/lava/matplotlib-cpp/)
+ GTK+ UVC Viewer: [http:///guvcview.sourceforge.net/](http://guvcview.sourceforge.net/)
+ boost 1.77.0: [https://www.boost.org/](https://www.boost.org/)
+ MeshLab 2021.07: [https://www.meshlab.net/](https://www.meshlab.net/)

ただし、matplotlib-cppには、[このURL](https://github.com/ShizSak/matplotlib-cpp-with-3D-Scatter-Plot)にある機能拡張パッチを適用する必要があります。

用いるハードウェアは次のとおりです。
+ USBインタフェースおよびHDMIインタフェース付きのPC（HDMIではなくDisplayPortやDVIなどでも可）
+ HDMIインタフェース付きのプロジェクタ（HDMIではなくDisplayPortやDVIなどでも可、PCと合わせること）
+ UVCに対応する、USBインタフェース付きのカメラ

このフォルダにあるプログラムは、次のとおりです。

## 2.2節
- リスト2.2: ```projection_of_mesh_pattern.cpp```
- リスト2.3: ```makefile```
- リスト2.10: ```camera_setting_and_capturing.py```
- リスト2.13: ```projection_of_grey_pattern.cpp```
- リスト2.14: ```camera_geometrical_calibration.py```
- リスト2.17: ```LUT_generation.cpp```

## 2.3節
- リスト2.19: ```3D_measurement.cpp```

以上
