#include "Intersection.h"
#include "SceneObject.h"


using namespace std;

const Intersection Intersection::noIntersection(NULL, Vector3::zero, Vector3::zero, -1);
const float Intersection::minIntersectionDistance = 0.0001f;

Intersection::Intersection():
	m_object(NULL),
	m_distance(0)
{
}

Intersection::Intersection(SceneObject *obj, const Vector3& point, const Vector3& normal, float distance):
	m_object(obj),
	m_point(point),
	m_normal(normal),
	m_distance(distance)
{
}

SceneObject *Intersection::obj() const
{
	return m_object;
}

Vector3 Intersection::normal() const
{
	return m_normal;
}

Vector3 Intersection::point() const
{
	return m_point;
}

float Intersection::distance() const
{
	return m_distance;
}

bool Intersection::intersects() const
{
	return m_distance > minIntersectionDistance; 
}

Intersection::operator bool() const
{
	return intersects();
}