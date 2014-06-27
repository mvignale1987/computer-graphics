#include "AABB.h"
#include <cfloat>

#define swap(x, y) { float temp = x; x = y; y = temp; }

AABB::AABB():
	minx(-FLT_MAX),
	miny(-FLT_MAX),
	minz(-FLT_MAX),
	maxx(+FLT_MAX),
	maxy(+FLT_MAX),
	maxz(+FLT_MAX)
{
}

AABB::AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz):
	minx(minx),
	miny(miny),
	minz(minz),
	maxx(maxx),
	maxy(maxy),
	maxz(maxz)
{
}

bool AABB::intersects(const Ray& r) const
{
	Vector3 orig = r.origin();
	Vector3 dir = r.direction();

	float tmin = (minx - orig.x()) / dir.x();
	float tmax = (maxx - orig.x()) / dir.x();
	if (tmin > tmax)
		swap(tmin, tmax);
	float tymin = (miny - orig.y()) / dir.y();
	float tymax = (maxy - orig.y()) / dir.y();
	if (tymin > tymax)
		swap(tymin, tymax);
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	float tzmin = (minz - orig.z()) / dir.z();
	float tzmax = (maxz - orig.z()) / dir.z();
	if (tzmin > tzmax)
		swap(tzmin, tzmax);
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	/*
	if ((tmin > r.tmax) || (tmax < r.tmin))
		return false;
	if (r.tmin < tmin)
		r.tmin = tmin;
	if (r.tmax > tmax)
		r.tmax = tmax;*/
	return true;
}
