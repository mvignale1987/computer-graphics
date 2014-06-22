#include "ImageBlock.h"
#include <sstream>
#include <iomanip>

using namespace std;

ImageBlock::ImageBlock():
	m_x(0), m_y(0), w(0), h(0)
{
}

ImageBlock::ImageBlock(unsigned int x, unsigned int y, unsigned int width, unsigned int height):
	m_x(x), m_y(y), w(width), h(height)
{
}


unsigned int ImageBlock::x() const
{
	return m_x;
}

unsigned int ImageBlock::y() const
{
	return m_y;
}

unsigned int ImageBlock::width() const
{
	return w;
}

unsigned int ImageBlock::height() const
{
	return h;
}

string ImageBlock::toString() const
{
	stringstream ss;
	ss << "<x: " << setw(4) << m_x << " , y: " << setw(4) << m_y << ", w: " << setw(3) << w << ", h: " << setw(3) << h << ">";
	return ss.str();
}