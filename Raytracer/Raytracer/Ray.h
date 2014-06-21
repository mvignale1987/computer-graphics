#pragma once

#include "Vector3.h"

class Ray
{
private:
	Vector3 m_direction;
	Vector3 m_origin;
public:
	Ray();
	Ray(const Vector3& direction, const Vector3& origin);

	Vector3 direction() const;
	Vector3 origin() const;
};

