#include "Ray.h"


Ray::Ray()
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction):
	m_origin(origin),
	m_direction(direction)
{
}

Vector3 Ray::direction() const
{
	return m_direction;
}

Vector3 Ray::origin() const
{
	return m_origin;
}

