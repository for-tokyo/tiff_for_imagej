/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#ifndef CV_TIFF_IMAGEJ_H_
#define CV_TIFF_IMAGEJ_H_

#include <opencv2/opencv.hpp>
#include "tiff_imagej.h"

void cv_write_tiff(const char *file, const cv::Mat &img, int flag = TI_BGR);


#endif //CV_TIFF_IMAGEJ_H_

