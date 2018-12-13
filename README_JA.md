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

write_tiff(const char *file, const void *data, int width, int height, int nChannels,
			   int type, int flag)
         
| name | description |
----|---- 
| file | filename |
| data | pointer of data. <br>Only interleaved is supported for a multi channel image. |
| width | width of image |
| nChannels | the number of channels |
| type | type of image |
| flag | to write image as color |

## Write cv::Mat as tiff file
Use the following files.
- tiff_imagej.cpp
- tiff_imagej.h
- cv_tiff_imagej.cpp	
- cv_tiff_imagej.h

void cv_write_tiff(const char *file, const cv::Mat &img, int flag = TI_BGR)

| name | description |
----|---- 
| file | filename |
| img | Mat object |
| flag | to write image as color |

