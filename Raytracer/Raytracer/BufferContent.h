#pragma once

#include "Vector3.h"

struct BufferContent 
{
	unsigned char r,g,b,a;

	BufferContent(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = UCHAR_MAX);
	BufferContent(const Vector3& color, unsigned char a = UCHAR_MAX);
};
