#pragma once
#include "ShapeDefinition.h"

class Sphere: public ShapeDefinition
{
private:
	float m_radius;
public:
	Sphere();
	Sphere(const std::string &id, float r);
	float radius();

	Intersection intersection(const Ray& ray);
};

