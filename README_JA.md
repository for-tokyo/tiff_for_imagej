# tiff_imagej
[日本語REAME](https://github.com/for-tokyo/tiff_imagej/README_JA.md)

ImageJ用にTiffファイル出力を行うためのライブラリです。

ライセンスは[the Unlicens](http://unlicense.org/)です。

制約なくこのプログラムの全部または一部を利用できます。

## 依存ライブラリ
- libtiff


## 画像データ（ポインタ）からTiffを出力
以下のファイルを自分のプロジェクトで利用してください。
- tiff_imagej.cpp
- tiff_imagej.h	

```
#include "tiff_imagej.cpp"

...
float *img_ptr = ....;
int width = ...;
int height = ...;
int nChannels = 3;

// インターリーブ形式の画素並び(BGRBGRBGR...)
for (...) {
   img_ptr[3*(x+y*width)+0] = Bval;
   img_ptr[3*(x+y*width)+1] = Gval; 
   img_ptr[3*(x+y*width)+1] = Rval; 
}

write_tiff("out.tiff", img_ptr, width, height, nChannnels, TI_32F, TI_RGB);
```
write_tiff(const char *file, const void *data, int width, int height, int nChannels,
			   int type, int flag)


| 名前 | 説明 |
----|---- 
| file | ファイル名 |
| data | 画像データへのポインタ. <br> 2チャンネル以上の場合、インターリーブ形式のみ対応しています。 |
| width | 画像幅 |
| height | 画像高さ |
| nChannels | チャンネル数 |
| type | 画像の型（ビット深度） |
| flag | フラグ（カラー画像用） |

typeは画素の型を指定します。

| 定数 | 説明 |
---- | ----
| TI_8U | 符号なし8ビット整数(unsigned char) |
| TI_16U | 符号なし16ビット整数(unsigned short) |
| TI_32S | 符号あり32ビット整数(int) |
| TI_32F | 単精度浮動小数(float) |
| TI_64F | 倍精度浮動小数(double) |

flagはnChannels==3のときのみ適用されます。

| 定数 | 説明 |
---- | ----
| TI_GRAY | グレースケール画像として出力します |
| TI_RGB | dataはRGB並びとしてカラー画像で出力します |
| TI_BGR | dataはBGR並びとしてカラー画像で出力します |


## Write cv::Mat as tiff file
OpenCVのcv::MatをTiff出力します。以下のファイルを自分のプロジェクトで利用してください。
- tiff_imagej.cpp
- tiff_imagej.h
- cv_tiff_imagej.cpp	
- cv_tiff_imagej.h

void cv_write_tiff(const char *file, const cv::Mat &img, int flag = TI_BGR)

| name | description |
----|---- 
| file | ファイル名 |
| img | cv::Mat オブジェクト |
| flag | フラグ（カラー画像用）  |

```
#include "cv_tiff_imagej.h"

...

cv::Mat img(width, height, CV_32FC3);
...
// OpenCVはBGR並びがデフォルトです。
cv_write_tiff("out.tiff", img, TI_BGR); 
```

