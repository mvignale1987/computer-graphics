#include "Intersection.h"
#include "SceneObject.h"


using namespace std;

const Intersection Intersection::noIntersection(NULL, -1);
const float Intersection::minIntersectionDistance = 0.0001f;

Intersection::Intersection():
	m_object(NULL),
	m_distance(0)
{
}

Intersection::Intersection(SceneObject *obj, float distance):
	m_object(obj),
	m_distance(distance)
{
}

SceneObject *Intersection::obj() const
{
	return m_object;
}

float Intersection::distance() const
{
	return m_distance;
}

bool Intersection::intersects() const
{
	return m_distance > 0; 
}

Intersection::operator bool() const
{
	return intersects();
}