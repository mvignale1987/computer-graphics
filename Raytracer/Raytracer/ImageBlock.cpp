#include "ImageBlock.h"
#include <sstream>

using namespace std;

ImageBlock::ImageBlock():
	m_x(0), m_y(0), w(0), h(0)
{
}

ImageBlock::ImageBlock(size_t x, size_t y, size_t width, size_t height):
	m_x(x), m_y(y), w(width), h(height)
{
}


size_t ImageBlock::x() const
{
	return m_x;
}

size_t ImageBlock::y() const
{
	return m_y;
}

size_t ImageBlock::width() const
{
	return w;
}

size_t ImageBlock::height() const
{
	return h;
}

string ImageBlock::toString() const
{
	stringstream ss;
	ss << "<x: " << m_x << " , y: " << m_y << ", w: " << w << ", h: " << h << ">";
	return ss.str();
}