#pragma once
#include "ShapeDefinition.h"

class Sphere: public ShapeDefinition
{
private:
	float radius;
public:
	Sphere();
	Sphere(float r);
	float getRadius();
};

