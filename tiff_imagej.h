/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#pragma once

//for type
#define TI_8U	0
#define TI_16U 	2
#define TI_32S	4
#define TI_32F	5
#define TI_64F	6

//for flag
#define TI_GRAY 0
#define TI_RGB	1
#define TI_BGR	2

void write_tiff(const char *file, const void *data, int width, int height, int nChannels,
			   int type, int flag = 0);

