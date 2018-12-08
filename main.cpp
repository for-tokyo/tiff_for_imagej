/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include "tiff_imagej.h"
#include "cv_tiff_imagej.h"


int main(int argc, char **argv)
{

	printf("tiff imagej test\n");

	if (argc == 1)
	{
		printf("no image file arg\n");
		return 0;
	}

	cv::Mat mat_8U_bgr = cv::imread(argv[1], 1);

	int w = mat_8U_bgr.cols;
	int h = mat_8U_bgr.rows;
	int c = mat_8U_bgr.channels();
	printf("%s : w = %d h = %d c = %d\n", argv[1], w, h, c);

	cv::Mat mat_16U_bgr, mat_32S_bgr, mat_32F_bgr, mat_64F_bgr;
	mat_8U_bgr.convertTo(mat_16U_bgr, CV_16U, 65535./255.);
	mat_8U_bgr.convertTo(mat_32S_bgr, CV_32S, 2147483647./255.);
	mat_8U_bgr.convertTo(mat_32F_bgr, CV_32F, 1./255.);
	mat_8U_bgr.convertTo(mat_64F_bgr, CV_64F, 1./255.);

	

	cv::Mat mat_8U_gray, mat_16U_gray, mat_32S_gray, mat_32F_gray, mat_64F_gray;

	cv::cvtColor(mat_8U_bgr, mat_8U_gray, CV_RGB2GRAY);
	mat_8U_gray.convertTo(mat_16U_gray, CV_16U, 65535./255.);
	mat_8U_gray.convertTo(mat_32S_gray, CV_32S, 2147483647./255.);
	mat_8U_gray.convertTo(mat_32F_gray, CV_32F, 1./255.);
	mat_8U_gray.convertTo(mat_64F_gray, CV_64F, 1./255.);

	// write primitive function (tiff_image.h)
	write_tiff("out_8U_rgb.tiff", mat_8U_bgr.ptr(), w, h, c, TI_8U, TI_BGR);
	write_tiff("out_16U_rgb.tiff", mat_16U_bgr.ptr(), w, h, c, TI_16U, TI_BGR);
	write_tiff("out_32S_rgb.tiff", mat_32S_bgr.ptr(), w, h, c, TI_32S, TI_BGR);
	write_tiff("out_32F_rgb.tiff", mat_32F_bgr.ptr(), w, h, c, TI_32F, TI_BGR);
	write_tiff("out_64F_rgb.tiff", mat_64F_bgr.ptr(), w, h, c, TI_64F, TI_BGR);

	write_tiff("out_8U_gray.tiff", mat_8U_gray.ptr(), w, h, 1, TI_8U);
	write_tiff("out_16U_gray.tiff", mat_16U_gray.ptr(), w, h, 1, TI_16U);
	write_tiff("out_32S_gray.tiff", mat_32S_gray.ptr(), w, h, 1, TI_32S);
	write_tiff("out_32F_gray.tiff", mat_32F_gray.ptr(), w, h, 1, TI_32F);
	write_tiff("out_64F_gray.tiff", mat_64F_gray.ptr(), w, h, 1, TI_64F);


	// write function for opencv (cv_tiff_image.h)
	cv_write_tiff("cv_8U_rgb.tiff", mat_8U_bgr);
	cv_write_tiff("cv_16U_rgb.tiff", mat_16U_bgr);
	cv_write_tiff("cv_32S_rgb.tiff", mat_32S_bgr);
	cv_write_tiff("cv_32F_rgb.tiff", mat_32F_bgr);
	cv_write_tiff("cv_64F_rgb.tiff", mat_64F_bgr);

	cv_write_tiff("cv_8U_gray.tiff", mat_8U_gray);
	cv_write_tiff("cv_16U_gray.tiff", mat_16U_gray);
	cv_write_tiff("cv_32S_gray.tiff", mat_32S_gray);
	cv_write_tiff("cv_32F_gray.tiff", mat_32F_gray);
	cv_write_tiff("cv_64F_gray.tiff", mat_64F_gray);

	cv::Mat mat_32F_4C = cv::Mat(256, 256, CV_32FC4);

	{
		float *ptr = (float*)mat_32F_4C.ptr();
		for (int y = 0; y < 256; y++)
		{
			for (int x = 0; x < 256; x++)
			{
				for (int c = 0; c < 4; c++)
				{
					float xc = 50+50*c;
					float yc = 50+50*c;
					float d2 = (x-xc)*(x-xc)+(y-yc)*(y-yc);
					float v = exp(-0.5*d2/(50*50));
					
					ptr[4*(256*y+x)+c] = v;
				}
			}
		}
		write_tiff("out_32FC4.tiff", ptr, 256, 256, 4, TI_32F);
		cv_write_tiff("cv_32FC4.tiff", mat_32F_4C);
	}


	return 0;
}
