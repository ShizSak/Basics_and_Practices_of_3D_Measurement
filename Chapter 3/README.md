# 第3章 LiDARを使って手軽に3次元計測実験
## 全体説明
このフォルダには3次元計測を行うためのプログラムを置いてあります。これらのプログラムはPythonで書かれており、中でOSSを活用していますので、必要なOSSを適宜インストールしてください。

## 3.3節
+ リスト3.5: ```LiDAR.py```
+ リスト3.8: ```polar.py```
+ リスト3.10: ```radar.py```

## 3.4節
3.4章では次の機材を用いて3次元計測を行います。

+ USBインタフェース付きのPC

次に、プログラムで直接呼び出すOSSに加え、使用するOSやアプリケーションを挙げています。なお、依存関係にあるOSSを追加でインストールする必要な場合があります。

Ubuntu Desktop 20.04LTS: https://jp.ubuntu.com/
Python 3.8: https://www.python.org/
Numpy 1.21.1: https://numpy.org/
OpenCV 4.5: https://opencv.org/
kivy 2.0.0: https://kivy.org/
Matplotlib 3.4.3: https://matplotlib.org/
matplotlib-cpp: https://github.com/lava/matplotlib-cpp/
GTK+ UVC Viewer: http:///guvcview.sourceforge.net/
boost 1.77.0: https://www.boost.org/
MeshLab 2021.07: https://www.meshlab.net/
上に示したmatplotlib-cppには、次のURLにあるset_zlim()関数及びscatter3()関数追加パッチを適用する必要があります。

https://github.com/ShizSak/matplotlib-cpp-with-3D-Scatter-Plot
このフォルダに置いてあるプログラムは、次のとおりです。詳細については本文を参照してください。


+ リスト3.22: ```extract_trajectory.py```

以上
