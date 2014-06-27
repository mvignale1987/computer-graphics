#pragma once

#include "Ray.h"

class AABB
{
public:
	float minx, miny, minz, maxx, maxy, maxz;

	AABB();
	AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz);

	bool intersects(const Ray& r) const;
};

