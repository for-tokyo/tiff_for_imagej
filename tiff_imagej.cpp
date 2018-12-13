/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#include <tiffio.h>
#include <stdlib.h>
#include <memory.h>
#include "tiff_imagej.h"

static int get_bytes_per_channel(int type)
{
	if (type == TI_8U)
	{
		return 1;
	}
	else if (type == TI_16U)
	{
		return 2;
	}
	else if (type == TI_32S)
	{
		return 4;

	}
	else if (type == TI_32F)
	{
		return 4;
	}
	else if (type == TI_64F)
	{
		return 8;
	}
	return 1;
}

static int _write_tiff_onepage(TIFF *tif, const void *data, int width, int height, int nChannels,
							   int type)
{
	int row_strip;
	int image_size;

	TIFFSetField(tif, TIFFTAG_SUBFILETYPE, 0);
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);


	if (type == TI_8U)
	{
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	}
	else if (type == TI_16U)
	{
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	}
	else if (type == TI_32S)
	{
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);

	}
	else if (type == TI_32F)
	{
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
		TIFFSetField(tif, TIFFTAG_SMINSAMPLEVALUE, 0.f);
		TIFFSetField(tif, TIFFTAG_SMAXSAMPLEVALUE, 1.0f);
	}
	else if (type == TI_64F)
	{
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 64);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
		//TIFFSetField(tif, TIFFTAG_SMINSAMPLEVALUE, 0.f);
		//TIFFSetField(tif, TIFFTAG_SMAXSAMPLEVALUE, 1.0f);
	}

	if (nChannels == 1)
	{
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	}
	else if (nChannels == 3)
	{
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	}

	int bytes_per_channel = get_bytes_per_channel(type);
	int bytes_per_pix =  bytes_per_channel * nChannels;
	int bytes_per_row = width * bytes_per_pix;
	size_t buf_size = TIFFScanlineSize(tif);
	unsigned char *buf = (unsigned char *)malloc(buf_size);

	for (int y = 0; y < height; y++)
	{
		memcpy(buf, (char *)data + bytes_per_row * y, bytes_per_row);
		if (TIFFWriteScanline(tif, buf, y, 0) < 0)
			break;
	}

	free(buf);

	return 0;
}

//BGR interleaving to RGB interleaving
template<typename T>
static T *create_rgb_data(const void *src, int width, int height)
{
	T *srcT = (T*)src;
	T *dst = (T*)malloc(sizeof(T)*width*height*3);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dst[3*(width*y+x)+2] = srcT[3*(width*y+x)+0];
			dst[3*(width*y+x)+1] = srcT[3*(width*y+x)+1];
			dst[3*(width*y+x)+0] = srcT[3*(width*y+x)+2];
		}
	}

	return dst;
}

//BGR interleaving to RGB interleaving
static void *create_rgb_data(const void *src, int width, int height, int type)
{
	if (type == TI_8U)
	{
		return create_rgb_data<unsigned char>(src, width, height);
	}
	else if (type == TI_16U)
	{
		return create_rgb_data<unsigned short>(src, width, height);
	}
	else if (type == TI_32S)
	{
		return create_rgb_data<int>(src, width, height);
	}
	else if (type == TI_32F)
	{
		return create_rgb_data<float>(src, width, height);
	}
	else if (type == TI_64F)
	{
		//not used
		return create_rgb_data<double>(src, width, height);
	}
	return 0;
}

template<typename T>
static T *create_one_channel_data(const void *src, int width, int height, int nChannels, int c)
{
	T *srcT = (T*)src;
	T *dst = (T*)malloc(sizeof(T)*width*height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dst[width*y+x] = srcT[nChannels*(width*y+x)+c];
		}
	}
	return dst;
}

static void *create_one_channel_data(const void *src, int width, int height, int nChannels, int type, int c)
{
	if (type == TI_8U)
	{
		return create_one_channel_data<unsigned char>(src, width, height, nChannels, c);
	}
	else if (type == TI_16U)
	{
		return create_one_channel_data<unsigned short>(src, width, height, nChannels, c);
	}
	else if (type == TI_32S)
	{
		return create_one_channel_data<int>(src, width, height, nChannels, c);
	}
	else if (type == TI_32F)
	{
		return create_one_channel_data<float>(src, width, height, nChannels, c);
	}
	else if (type == TI_64F)
	{
		//not used
		return create_one_channel_data<double>(src, width, height, nChannels, c);
	}
	return 0;
}

//BGR interleaving to RGB separate (for Double)
template<typename T>
static T *create_separate_from_bgr(const void *src, int width, int height)
{
	T *srcT = (T*)src;
	T *dst = (T*)malloc(sizeof(T)*width*height*3);
	const int rOffset = 0;
	const int gOffset = width*height;
	const int bOffset = 2*width*height;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//R
			dst[width*y+x+rOffset] = srcT[3*(width*y+x)+2];
			//G
			dst[width*y+x+gOffset] = srcT[3*(width*y+x)+1];
			//B
			dst[width*y+x+bOffset] = srcT[3*(width*y+x)+0];
		}
	}
	return dst;
}

template<typename T>
static T *create_separate_data(const void *src, int width, int height, int nChannels)
{
	T *srcT = (T*)src;
	T *dst = (T*)malloc(sizeof(T)*width*height*nChannels);
	const int cOffset = width*height;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int c = 0; c < nChannels; c++)
			{
				dst[width*y+x+c*cOffset] = srcT[nChannels*(width*y+x)+c];
			}
		}
	}
	return dst;
}


static void *create_separate_data(const void *src, int width, int height, int nChannels,
					  int type)
{
	if (type == TI_8U)
	{
		return create_separate_data<unsigned char>(src, width, height, nChannels);
	}
	else if (type == TI_16U)
	{
		return create_separate_data<unsigned short>(src, width, height, nChannels);
	}
	else if (type == TI_32S)
	{
		return create_separate_data<int>(src, width, height, nChannels);
	}
	else if (type == TI_32F)
	{
		return create_separate_data<float>(src, width, height, nChannels);
	}
	else if (type == TI_64F)
	{
		return create_separate_data<double>(src, width, height, nChannels);
	}
	return 0;
}

void write_tiff(const char *file, const void *data, int width, int height, int nChannels,
			   int type, int flag)
{
	TIFF *tif = TIFFOpen(file, "wb");
	if (!tif)
	{
		return;
	}


	if (nChannels == 1)
	{
		//write one page
		_write_tiff_onepage(tif, data, width, height, nChannels, type);
	}
	else if (flag == TI_RGB && nChannels == 3)
	{
		if (type == TI_8U || type == TI_16U)
		{
			//write one page as rgb interleaving
			_write_tiff_onepage(tif, data, width, height, nChannels, type);
		}
		else
		{
			//write multipage as rgb color
			for (int c = 0; c < nChannels; c++)
			{
				//RGB interleaving -> RGB separate
				void *sep_data = create_one_channel_data(data, width, height, nChannels, type, c);
				_write_tiff_onepage(tif, sep_data, width, height, 1, type);
				free(sep_data);
				// for imagej mutipage as color
				const char *str  = "ImageJ=1.52d\nimages=3\nchannels=3\nhyperstack=true\nmode=composite\nloop=false";
				TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, str);
				TIFFWriteDirectory(tif);
			}
		}
	}
	else if (flag == TI_BGR && nChannels == 3)
	{
		if (type == TI_8U || type == TI_16U)
		{
			//BGR -> RGB interleaving
			void *rgbData = create_rgb_data(data, width, height, type);
			//write one page as rgb interleaving
			_write_tiff_onepage(tif, rgbData, width, height, nChannels, type);
			free(rgbData);
		}
		else
		{
			//write multipage as rgb color
			for (int c = 0; c < 3; c++)
			{
				//BGR interleaving -> BGR separate
				void *sep_data = create_one_channel_data(data, width, height, nChannels, type, 2-c);
				_write_tiff_onepage(tif, sep_data, width, height, 1, type);
				free(sep_data);
				// for imagej mutipage as color
				const char *str  = "ImageJ=1.52d\nimages=3\nchannels=3\nhyperstack=true\nmode=composite\nloop=false";
				TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, str);
				TIFFWriteDirectory(tif);
			}
		}
	}
	else
	{
		//write multipage
		for (int c = 0; c < nChannels; c++)
		{
			void *sep_data = create_one_channel_data(data, width, height, nChannels, type, c);
			_write_tiff_onepage(tif, sep_data, width, height, 1, type);
			free(sep_data);
			TIFFWriteDirectory(tif);
		}
	}

	TIFFClose(tif);
	return;
}
