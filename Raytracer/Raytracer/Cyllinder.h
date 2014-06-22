#pragma once

#include "Vector3.h"
#include "ShapeDefinition.h"

class Cyllinder: public ShapeDefinition
{
private:
	float	m_height;
	float	m_radius;

public:
	Cyllinder();
	Cyllinder(const std::string &id, float cHeight, float cRadius);
	float height() const;
	float radius() const;

	Intersection intersection(const Ray& ray);
};