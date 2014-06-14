#include "Cyllinder.h"
 
Cyllinder::Cyllinder():
	pos(Vector3::zero),
	height(1),
	radius(1)
{
}

Cyllinder::Cyllinder(const Vector3 &pos, float cHeight, float cRadius):
	pos(pos),
	height(cHeight),
	radius(cRadius)
{
}

Vector3 Cyllinder::position() const
{
	return pos;
}

float Cyllinder::getHeight() const
{
	return height;
}
float Cyllinder::getRadius() const
{
	return radius;
}