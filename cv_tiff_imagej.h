/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#pragma once
#include <opencv2/opencv.hpp>
#include "tiff_imagej.h"

void cv_write_tiff(const char *file, const cv::Mat &img, int flag = TI_BGR);
