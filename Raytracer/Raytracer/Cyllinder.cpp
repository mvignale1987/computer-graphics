#include "Cyllinder.h"
 
Cyllinder::Cyllinder():
	ShapeDefinition(Shape::CYLLINDER, ""),
	m_height(1),
	m_radius(1)
{
}

Cyllinder::Cyllinder(const std::string &id, float cHeight, float cRadius):
	ShapeDefinition(Shape::CYLLINDER, id),
	m_height(cHeight),
	m_radius(cRadius)
{
}

float Cyllinder::height() const
{
	return m_height;
}
float Cyllinder::radius() const
{
	return m_radius;
}

Intersection Cyllinder::intersection(const Ray& ray)
{
	return Intersection::noIntersection;
}