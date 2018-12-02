/*
	Copyright 2018-, Yuki TOKYO (@for-tokyo)
	Licensed under Unlicense (http://unlicense.org)
*/

#pragma once

#define TI_8U	0
#define TI_16U 	2
#define TI_32S	4
#define TI_32F	5
#define TI_64F	6

#define TI_GRAY 0
#define TI_RGB	1
#define TI_BGR	2

int write_tiff(const char *file, void *data, int width, int height, int nChannels,
			   int type, int flags = 0);

