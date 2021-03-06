# 第3章 LiDARを使って手軽に3次元計測実験
## 全体説明
このフォルダには3章で説明した、3次元計測を行うためのPythonによるサンプルプログラムを置いています。これらのプログラムを動かすためには、いくつかのOSSをインストールする必要があります。また、3.2節のコラムでふれた、Intel RealSense用のLEGOマウントデータも置いてあります。

このフォルダに置いてあるプログラムとデータは、次の通りです。詳細については本文を参照してください。

## 3.2節
Intel RealSenseのうち、D415・D435・L515用のLEGOマウントデータです。これらのデータは[竹久さま](https://github.com/aho1go)からご提供いただきました。なお、3Dプリンタのプリント精度はLEGOブロック製造における精度には及ばないために篏合で不具合がでる可能性がある、またバリ除去なども含めてプリント後に処理が必要になる可能性があることに留意して利用ください。
+ [Intel RealSense D415・D435共通LEGOマウント](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/RealsenseD415_435-LEGO.stl)
+ [Intel RealSense L515 LEGOマウント](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/RealsenseL515-LEGO.stl)

## 3.3節
3.3節では次の機材を用いて3次元計測を行います。
+ USBインタフェース付きのPC
+ Raspberry Pi Zero WH
+ ST micro VL53L0X（TOFチップ）が搭載されたTOFセンサ

PC上で使用するOSやアプリケーションに加え、サンプルプログラムから直接呼び出すOSSを次に挙げます。これらが依存するOSSの追加インストールが必要となる場合があります。
+ PC用
  - Ubuntu Desktop 20.04LTS: [https://jp.ubuntu.com/](https://jp.ubuntu.com/)
  - Python 3.8: [https://www.python.org/](https://www.python.org/)
  - NumPy 1.21.4: [https://numpy.org/](https://numpy.org/)
  - matplotlib 3.5.0: [https://matplotlib.org/](https://matplotlib.org/)
  - pandas 1.3.4: [https://pandas.pydata.org/](https://pandas.pydata.org/)
  - Kivy 2.0.0: [https://kivy.org/](https://kivy.org/)

こちらはRaspberry Pi Zero MH用です。
+ Raspberry Pi Zero WH用
  - Raspberry Pi OS: [https://www.raspberrypi.com/software/operating-systems/](https://www.raspberrypi.com/software/operating-systems/)
  - Python 3.8: [https://www.python.org/](https://www.python.org/)
  - VL53L0X-python: [https://github.com/pimoroni/VL53L0X-python](https://github.com/pimoroni/VL53L0X-python)
  - pigpio: [https://github.com/joan2937/pigpio](https://github.com/joan2937/pigpio)

3.3節に掲載するサンプルプログラムは次の通りです。これらのプログラムは[竹久さま](https://github.com/aho1go)からご提供いただきました。
+ リスト3.5: [```LiDAR.py```](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/LiDAR.py)
+ リスト3.8: [```polar.py```](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/polar.py)
+ リスト3.10: [```radar.py```](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/radar.py)

なお、[竹久さま](https://github.com/aho1go)の部品入手先を、[```RadarLiDAR.txt```](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/RadarLiDAR.txt)に参考情報として記載しています。記載情報はあくまでも2022年5月時点のものであることに留意してください。

## 3.4節・3.5節
3.4節・3.5節では次の機材を用いて3次元計測を行います。

+ USB3インタフェース付きのPC
+ Microsoft(R) Azure Kinect(TM) DK（3次元計測装置）

使用するOSやアプリケーションに加え、サンプルプログラムから直接呼び出すOSSを次に挙げます。これらが依存するOSSの追加インストールが必要となる場合があります。
+ Ubuntu Desktop 20.04LTS: [https://jp.ubuntu.com/](https://jp.ubuntu.com/)
+ Python 3.8: [https://www.python.org/](https://www.python.org/)
+ Open3D 0.13: [http://www.open3d.org/](http://www.open3d.org/)
+ MeshLab 2021.07: [https://www.meshlab.net/](https://www.meshlab.net/)

3.4節では掲載するサンプルプログラムはありません。3.5節に掲載するサンプルプログラムは次の通りです。
+ リスト3.22: [```extract_trajectory.py```](https://github.com/ShizSak/Basics_of_3D_Measurement/blob/main/Chapter%203/extract_trajectory.py)

以上
