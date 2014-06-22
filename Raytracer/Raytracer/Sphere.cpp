#include "Sphere.h"

Sphere::Sphere():
	ShapeDefinition(Shape::SPHERE, ""),
	m_radius(1)
{
}

Sphere::Sphere(const std::string &id, float r):
	ShapeDefinition(Shape::SPHERE, id),
	m_radius(r)
{
}
float Sphere::radius(){
	return m_radius;

}

Intersection Sphere::intersection(const Ray& ray)
{
	return Intersection::noIntersection;
}