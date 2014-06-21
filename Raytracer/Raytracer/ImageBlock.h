#pragma once

#include <string>

class ImageBlock
{
private:
	size_t m_x;
	size_t m_y;
	size_t h;
	size_t w;
public:
	ImageBlock();
	ImageBlock(size_t x, size_t y, size_t width, size_t height);
	size_t x() const;
	size_t y() const;
	size_t width() const;
	size_t height() const;

	std::string toString() const;
};

