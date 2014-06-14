#pragma once

#include "Vector3.h"
#include "ShapeDefinition.h"

class Cyllinder: public ShapeDefinition
{
private:
	Vector3 pos;
	float	height;
	float	radius;

public:
	Cyllinder();
	Cyllinder(const Vector3 &pos, float cHeight, float cRadius);

	Vector3 position() const;

	float getHeight() const;
	float getRadius() const;
};