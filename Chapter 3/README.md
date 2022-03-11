# 第3章 LiDARを使って手軽に3次元計測実験
## 全体説明
このフォルダには3次元計測を行うためのプログラムを置いてあります。これらのプログラムはPythonで書かれており、中でOSSを活用していますので、必要なOSSを適宜インストールしてください。

このフォルダに置いてあるプログラムは、次のとおりです。詳細については本文を参照してください。

## 3.3節
+ リスト3.5: ```LiDAR.py```
+ リスト3.8: ```polar.py```
+ リスト3.10: ```radar.py```

## 3.4節・3.5節
3.4節では次の機材を用いて3次元計測を行います。

+ USB3インタフェース付きのPC
+ Microsoft社 Azure Kinect DK（3次元計測装置）

次に、プログラムで直接呼び出すOSSに加え、使用するOSやアプリケーションを挙げています。なお、依存関係にあるOSSを追加でインストールする必要な場合があります。
+ Ubuntu Desktop 20.04LTS: [https://jp.ubuntu.com/](https://jp.ubuntu.com/)
+ Python 3.8: [https://www.python.org/](https://www.python.org/)
+ Open3D 0.13: [https://www.open3d.org/](https://www.open3d.org/)
+ MeshLab 2021.07: [https://www.meshlab.net/](https://www.meshlab.net/)

3.4節に掲載するプログラムは、次のとおりです。
+ リスト3.22: [```extract_trajectory.py```](https://github.com/ShizSak/Basics-of-3D-Measurement/blob/main/Chapter%203/extract_trajectory.py)

以上
