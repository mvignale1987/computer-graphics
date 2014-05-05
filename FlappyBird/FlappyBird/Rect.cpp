#include "Rect.h"

Rect::Rect()
{
	borders[0] = borders[1] = borders[2] = borders[3] = 0;
}

Rect::Rect(float top, float left, float bottom, float right)
{
	borders[0] = top;
	borders[1] = left;
	borders[2] = bottom;
	borders[3] = right;
}

Rect::Rect(int top, int left, int bottom, int right)
{
	borders[0] = (float) top;
	borders[1] = (float) left;
	borders[2] = (float) bottom;
	borders[3] = (float) right;
}

float Rect::top() const
{
	return borders[0];
}

float Rect::left() const
{
	return borders[1];
}

float Rect::bottom() const
{
	return borders[2];
}

float Rect::right() const
{
	return borders[3];
}

bool Rect::contains(float x, float y) const
{
	return x >= left() && x <= right() && y >= top() && y <= bottom();
}

bool Rect::contains(int x, int y) const
{
	return x >= left() && x <= right() && y >= top() && y <= bottom();
}