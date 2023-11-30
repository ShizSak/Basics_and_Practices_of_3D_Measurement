# ゼロからわかる3次元計測 ー3Dスキャナ，LiDARの原理と実践ー
## 坂本静生著，オーム社
## 2022年6月10日 第1版第1刷発行
## 2023年11月10日 第1版第2刷発行

このリポジトリには、オーム社から発売の「ゼロからわかる3次元計測」に掲載したサンプルプログラムとデータを置いています。

「ゼロからわかる3次元計測」では、2章と3章で3次元計測を実践します。
+ [2章 アクティブ型三角測量による3次元計測](https://github.com/ShizSak/Basics_of_3D_Measurement/tree/main/Chapter%202)
+ [3章 LiDARを使って手軽に3次元計測実験](https://github.com/ShizSak/Basics_of_3D_Measurement/tree/main/Chapter%203)

なお、3.2節のコラムで紹介したデータ及び3.3節のプログラムは、[竹久さま](https://github.com/aho1go)からご提供していただきました。

## 書誌情報
+ 出版社: オーム社 (2022/6/14)
+ 発売日: 2022/6/14
+ 言語: 日本語
+ 単行本: 232ページ
+ ISBN-10: 4274228827
+ ISBN-13: 978-4274228827
+ →[オーム社の書籍ページ](https://www.ohmsha.co.jp/book/9784274228827/)

![表紙](https://github.com/ShizSak/Basics_and_Practices_of_3D_Measurement/blob/main/FrontCover.jpg)

## 内容紹介
3Dスキャナ、LiDARなどの3次元計測技術がゼロからわかる

本書は、物理空間中にある物体の3次元形状を映像データとして取り込む技術である3次元計測技術について、読者の方々に手を動かしながら、その原理を理解してもらう本です。

3次元計測技術を理解するために必要な知識を最初の一歩から順を追って解説しています。物体の3次元情報をコンピュータにインプットするためにはどういったことが必要なのか、そのためにはどのような方法が使われているのか、その数理モデルはどうやって導かれるのか、そして、簡単に実践してみるところまでを説明しています。

本書にひととおり目を通し、実際に手を動かしてみることで、さまざまな場面で応用できる基礎力を養っていただけるはずです。

## 目次
+ 第1章 3次元計測技術の基礎
  - 1.1 カメラのみによる計測
  - 1.2 光パターン投影で高精度計測
  - 1.3 反射光の到達時間で計測（TOF）
+ 第2章 アクティブ型三角測量による3次元計測
  - 2.1 カメラとプロジェクタのプロパティ設定
  - 2.2 カメラとプロジェクタの設置
  - 正弦波位相シフト法による3次元計測
+ 第3章 LiDARを使って手軽に3次元計測実験
  - 3.1 LiDAR搭載タブレットによる3次元計測
  - 3.2 市販の3次元計測装置の活用
  - 3.3 レーダ型LiDARを試作する
  - 3.4 3次元計測装置を動かしながらRGB-Dデータを取得
  - 3.5 時系列RGB-Dデータの統合
+ 第4章 3次元計測装置の設計と開発
  - 4.1 3次元計測装置の要件を洗い出す
  - 4.2 アクティブ型三角測量の設計と開発
  - 4.3 TOFセンサ／LiDARの設計と開発
  - 4.4 将来の3次元計測技術

## 正誤表（第1版第1刷）
+ 2022年6月8日
  - 複数個所: 「Ubuntu」→「ubuntu」
  - 複数個所: 「Raspberry Pi zero WH」→「Raspberry Pi Zero WH」
  - p.148 図3.18: 2つの吹き出しと図中のメニューの位置がずれている（吹き出し位置をもう少し下に移動）
  - p.157 (2)冒頭: 「続けてI<sup>2</sup>CとSPIにもとづく…」→「続けてI<sup>2</sup>Cにもとづく…」
  - p.190 上から3行目: 「具体的にいうと，Dense SLAMGUIのほうが，」→「具体的にいうと，DenseSLAMGUIのほうが，」（スペースを削除）
  - p.208 ＜海外＞リスト: 「PIMONORI」→「PIMORONI」
+ 2022年6月28日
  - p.137 図3.9: 「三角形の1辺を15cmまで」→「三角形の1辺を15mmまで」
+ 2022年7月3日
  - p.173 図3.36: 「ランダムな誤差の標準偏差」→「ランダムな誤差の標準偏差×2」、矢印位置はもう少し上で幅は狭い（誤差分布が正規分布の場合、計測値が平均±標準偏差内におさまるのが約68％（3回計測したら2回はこの範囲におさまる）、平均±2標準偏差におさまるのが約95％（20回計測したら19回はこの範囲におさまる））
+ 2022年7月15日
  - p.21 式(1.21): 一番上の式、「$J(x_1,t)$」→「$J(x_1,0)$」
  - p.26 式(1.37): 右辺、「$tan^{-1}[\frac{I(x_0^P,0) - I(x_0^P,\pi)}{I(x_0^P,\frac{\pi}{2}) - I(x_0^P,\frac{3\pi}{2})}]$」→「$tan^{-1}[\frac{-(I(x_0^P,\frac{\pi}{2}) - I(x_0^P,\frac{3\pi}{2}))}{I(x_0^P,0) - I(x_0^P,\pi)}]$」

## 正誤表（第1版第2刷）
+ （なし）

以上
