#pragma once

#include "Vector3.h"

class Ray
{
private:
	Vector3 m_origin;
	Vector3 m_direction;
public:
	Ray();
	Ray(const Vector3& origin, const Vector3& direction);

	Vector3 origin() const;
	Vector3 direction() const;
};

