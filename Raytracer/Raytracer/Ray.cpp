#include "Ray.h"


Ray::Ray()
{
}

Ray::Ray(const Vector3& direction, const Vector3& origin):
	m_direction(direction.normalized()),
	m_origin(origin)
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

