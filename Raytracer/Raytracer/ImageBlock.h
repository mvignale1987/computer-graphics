#pragma once

#include <string>

class ImageBlock
{
private:
	unsigned int m_x;
	unsigned int m_y;
	unsigned int h;
	unsigned int w;
public:
	ImageBlock();
	ImageBlock(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	unsigned int x() const;
	unsigned int y() const;
	unsigned int width() const;
	unsigned int height() const;

	std::string toString() const;
};

