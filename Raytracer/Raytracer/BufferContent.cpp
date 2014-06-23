#include "BufferContent.h"

BufferContent::BufferContent(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
	r(r), g(g), b(b), a(a)
{
}

BufferContent::BufferContent(const Vector3& color, unsigned char a):
	a(a)
{
	float r = color.x();
	float g = color.y();
	float b = color.z();
	
	if(r < 0)
		r = 0;
	if(r > 1)
		r = 1;
	this->r = (unsigned char) (r * 255.0f);

	if(g < 0)
		g = 0;
	if(g > 1)
		g = 1;
	this->g = (unsigned char) (g * 255.0f);

	if(b < 0)
		b = 0;
	if(b > 1)
		b = 1;
	this->b = (unsigned char) (b * 255.0f);
}
