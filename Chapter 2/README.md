# 第2章 アクティブ型三角測量による3次元計測
## 全体説明
2章では次の機材を用いて、正弦波格子位相シフト法に基づいて3次元計測を行います。
+ USBインタフェースおよびHDMIインタフェース付きのPC（HDMIではなくDisplayPortやDVIなどでも可）
+ HDMIインタフェース付きのプロジェクタ（HDMIではなくDisplayPortやDVIなどでも可、PCと合わせること）
+ UVCに対応する、USBインタフェース付きのカメラ

このフォルダには3次元計測を行うためのプログラムを置いてあります。これらのプログラムはOSSを活用していますので、適宜インストールしてください。次に、プログラムで直接呼び出すOSSに加え、使用するOSやアプリケーションを挙げています。必要に応じて、依存関係にあるOSSもインストールしてください。
+ Ubuntu Desktop 20.04LTS: [https://jp.ubuntu.com/](https://jp.ubuntu.com/)
+ Python 3.8: [https://www.python.org/](https://www.python.org/)
+ Numpy 1.21.1: [https://numpy.org/](https://numpy.org/)
+ OpenCV 4.5: [https://opencv.org/](https://opencv.org/)
+ kivy 2.0.0: [https://kivy.org/](https://kivy.org/)
+ Matplotlib 3.4.3: [https://matplotlib.org/](https://matplotlib.org/)
+ matplotlib-cpp: [https://github.com/lava/matplotlib-cpp/](https://github.com/lava/matplotlib-cpp/)
+ GTK+ UVC Viewer: [http:///guvcview.sourceforge.net/](http://guvcview.sourceforge.net/)
+ boost 1.77.0: [https://www.boost.org/](https://www.boost.org/)
+ MeshLab 2021.07: [https://www.meshlab.net/](https://www.meshlab.net/)

なお、上に示したmatplotlib-cppには、次のURLにある機能拡張パッチを適用する必要があります。
+ [https://github.com/ShizSak/matplotlib-cpp-with-3D-Scatter-Plot](https://github.com/ShizSak/matplotlib-cpp-with-3D-Scatter-Plot)

このフォルダに置いてあるプログラムは、次のとおりです。詳細については本文を参照してください。

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
