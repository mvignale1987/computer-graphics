#include "SceneObjectQuad.h"


SceneObjectQuad::SceneObjectQuad(Material *material, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d):
	SceneObject(material),
	a(a), b(b), c(c), d(d)
{
	Vector3 v1 = b - a;
	Vector3 v2 = c - a;
	normal = v1.cross(v2).normalized();
}

Intersection SceneObjectQuad::intersection(const Ray& r)
{
	Vector3 origin = r.origin();
	Vector3 direction = r.direction();
	float distance;
	float numerator = (a - origin) * normal;
	float denominator = direction * normal;
	if(numerator == 0)
		distance = 0; // r.origin lies on the plane
	else if(denominator == 0)
		return Intersection::noIntersection; // ray is parallel 
	else
		distance = numerator / denominator; // ray intersects on single place
	
	if(distance > 0.001f && distance < 1000.0f)
		return Intersection(this, distance);
	else 
		return Intersection::noIntersection;
}

Vector3 SceneObjectQuad::normalAt(const Ray& r, const Vector3& point)
{
	if((r.origin() - point) * normal < 0)
		return -normal;
	else
		return normal;
}
