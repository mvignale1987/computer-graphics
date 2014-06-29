#include "SceneObjectCyllinder.h"


SceneObjectCyllinder::SceneObjectCyllinder(Material *material, const Vector3& center, float radius, float height):
	SceneObject(material),
	m_center(center),
	m_radius(radius),
	m_height(height),
	m_aabb(m_center.x() - m_radius, m_center.y() - m_height, m_center.z() - m_radius, 
		m_center.x() + m_radius, m_center.y() + m_height, m_center.z() + m_radius)
{
}

Vector3 SceneObjectCyllinder::center() const
{
	return m_center;
}
float   SceneObjectCyllinder::radius() const
{
	return m_radius;
}

float   SceneObjectCyllinder::height() const
{
	return m_height;
}

Intersection SceneObjectCyllinder::intersection(const Ray& ray)
{
	//Compute A, B and C coefficients
	Vector3 d = ray.direction();
	Vector3 o = ray.origin() - m_center;
	//we will use the rect equations for the ray to intersect the cyllinder
	float a = d.x()*d.x() + d.z()*d.z();
	float b = 2 * o.x() * d.x() + 2 * o.z() * d.z();
	float c = o.x()*o.x() + o.z()*o.z() - (m_radius * m_radius);

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
		if (y1 >= 0 && y1 <= m_height)		// checks if we intersect the whole cyllinder or just the finite part
		{
			return Intersection(this, t1);			
		} 
		
		t0 = -o.y() / d.y();
		t1 = (m_height - o.y()) / d.y();

		if(t1 < 0)
			return Intersection::noIntersection;
		if(t0 < -0.01f && t1 > 0.01f)
			return Intersection(this, t1);
		else {
			return Intersection(this, t0);
		}
    }
    // else the intersection point is at t0 OR AT THE CAP!!!!
    else if(t0 > 0.01f)
    {
		
		float y0 =  o.y() + t0*d.y();
		float y1 =  o.y() + t1*d.y();
		if (y0 <= m_height && y1 >= m_height || y1 <= m_height && y0 >= m_height) // here we should improve the angle of the cap
		{
			//SPECIAL UPPER CAP INTERSECTION
			float t3 = (m_height -  o.y()) / d.y();
			return Intersection(this, t3);		
		}
		if (y0 <= 0 && y1 >= 0 /*|| y1 <= 0 && y0 >= 0*/)
		{
			//SPECIAL LOWER CAP INTERSECTION
			float t3 = (0.0f -  o.y()) / d.y();
			return Intersection(this, t3);		
		}
		if (y0 >= 0 && y0 <= m_height )
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

Vector3 SceneObjectCyllinder::normalAt(const Ray&, const Vector3& point)
{
	if (point.y() > m_center.y() && point.y() < m_center.y()+m_height)
		return point + Vector3(point.x()/m_radius,0.0f,point.z()/m_radius);
	else if (fabs(point.y() - m_center.y()) < 0.01f)
		return point + Vector3::down;
	else
		return point + Vector3::up;
}

AABB SceneObjectCyllinder::aabb()
{
	return m_aabb;
}

Vector2 SceneObjectCyllinder::textureCoordinatesAt(const Vector3&point)
{
	float x = point.x() - m_center.x();
	float z = point.z() - m_center.z();

	float s = acosf(x / m_radius) / (2.0f * ((float) M_PI));
	float t = z / m_height;
	return Vector2(s, t);
}

Vector3 SceneObjectCyllinder::xTextureVector(const Vector3&)
{
	return Vector3::right;
}

Vector3 SceneObjectCyllinder::yTextureVector(const Vector3&)
{
	return Vector3::up;
}

