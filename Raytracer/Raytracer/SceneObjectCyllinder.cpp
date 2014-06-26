#include "SceneObjectCyllinder.h"


SceneObjectCyllinder::SceneObjectCyllinder(Material *material, const Vector3& center, float radius, float height):
	SceneObject(material),
	center(center),
	radius(radius),
	height(height)
{
}

Intersection SceneObjectCyllinder::intersection(const Ray& ray)
{
	//Compute A, B and C coefficients
	Vector3 d = ray.direction();
	Vector3 o = ray.origin() - center;
	//we will use the rect equations for the ray to intersect the cyllinder
	float a = d.x()*d.x() + d.z()*d.z();
	float b = 2 * o.x() * d.x() + 2 * o.z() * d.z();
	float c = o.x()*o.x() + o.z()*o.z() - (radius * radius);

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
		

	// with these values we calculate y axis position of the ray and check if it collides the finite cyllinder
	// if t0 is less than zero, the intersection point is at t1
    if (t0 < -0.01f && t1 > 0.01f)
    {
		float y1 =  o.y() + t1*d.y();	//evaluate the ray on desired t value
		if (y1 >= 0 && y1 <= height)		// checks if we intersect the whole cyllinder or just the finite part
		{
			return Intersection(this, t1);			
		}
		else
		{
			return Intersection::noIntersection;
		}
    }
    // else the intersection point is at t0 OR AT THE CAP!!!!
    else if(t0 > 0.01f)
    {
		
		float y0 =  o.y() + t0*d.y();
		float y1 =  o.y() + t1*d.y();
		if (y0 <= height && y1 >= height || y1 <= height && y0 >= height)
		{
			//SPECIAL UPPER CAP INTERSECTION
			float t3 = (height -  o.y()) / d.y();
			return Intersection(this, t3);		
		}
		if (y0 <= 0 && y1 >= 0 || y1 <= 0 && y0 >= 0)
		{
			//SPECIAL LOWER CAP INTERSECTION
			float t3 = (0.0f -  o.y()) / d.y();
			return Intersection(this, t3);		
		}
		if (y0 >= 0 && y0 <= height )
		{
			return Intersection(this, t0);		
		}
		else
		{
			return Intersection::noIntersection;
		}
    }
	else
	{
		return Intersection::noIntersection;
	}
}

Vector3 SceneObjectCyllinder::normalAt(const Ray& r, const Vector3& point)
{
	
	if (point.y() > center.y() && point.y() < center.y()+height)
		return point + Vector3(point.x()/radius,0.0f,point.z()/radius);
	else if (point.y() == center.y())
		return point + Vector3::down;
	else if (point.y() == center.y()+height)
		return point + Vector3::up;
	
		
}