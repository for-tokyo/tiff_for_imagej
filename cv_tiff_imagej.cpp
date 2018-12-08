#include "tiff_imagej.h"
#include "cv_tiff_imagej.h"


void cv_write_tiff(const char *file, const cv::Mat &img)
{
	int cv_type = img.type();
	int t = cv_type & CV_MAT_DEPTH_MASK;
	int c = 1 + (cv_type >> CV_CN_SHIFT);
	int height = img.rows;
	int width = img.cols;


	int type = TI_8U;
	if (t == CV_8U)
	{
		type = TI_8U;
	}
	else if (t == CV_16U)
	{
		type = TI_16U;
	}
	else if (t == CV_32S)
	{
		type = TI_32S;
	}
	else if (t == CV_32F)
	{
		type = TI_32F;
	}
	else if (t == CV_64F)
	{
		type = TI_64F;
	}

	int flag = TI_GRAY;
	if (c == 3)
	{
		flag = TI_BGR;
	}

	write_tiff(file, img.ptr(), width, height, c, type, flag);

}

