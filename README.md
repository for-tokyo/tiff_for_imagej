# tiff_imagej
[日本語REAME](https://github.com/for-tokyo/tiff_imagej/blob/master/README_JA.md)

write a tiff file for imagej.

This source code is licensed under [the Unlicens](http://unlicense.org/).

You can use all or part of this source code without any constraints.


## Dependency
- libtiff


## Write image data as tiff file
Use the following files.
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


