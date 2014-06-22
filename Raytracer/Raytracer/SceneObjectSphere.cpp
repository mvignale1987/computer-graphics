#include "SceneObjectSphere.h"


SceneObjectSphere::SceneObjectSphere(Material *material, const Vector3& center, float radius):
	SceneObject(material),
	center(center),
	radius(radius)
{
}

Intersection SceneObjectSphere::intersection(const Ray& ray)
{
	//Compute A, B and C coefficients
	Vector3 d = ray.direction();
	Vector3 o = ray.origin() - center;

	float a = d * d;
	float b = 2 * o * d;
	float c = o*o - (radius * radius);

    //Find discriminant
    float disc = b * b - 4 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0)
		return Intersection::noIntersection;

    // compute q as described above
    float distSqrt = sqrtf(disc);
    float q;
    if (b < 0)
        q = (-b - distSqrt)/2.0f;
    else
        q = (-b + distSqrt)/2.0f;

    // compute t0 and t1
    float t0 = q / a;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1)
    {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < 0)
		return Intersection::noIntersection;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < -0.01f && t1 > 0.01f)
    {
		return Intersection(this, t1);
    }
    // else the intersection point is at t0
    else if(t0 > 0.01f)
    {
		return Intersection(this, t0);
    } else
	{
		return Intersection::noIntersection;
	}
}

Vector3 SceneObjectSphere::normalAt(const Vector3& p)
{
	return p - center;
}