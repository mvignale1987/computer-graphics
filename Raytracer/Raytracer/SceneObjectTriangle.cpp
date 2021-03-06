#include "SceneObjectTriangle.h"

using namespace std;

SceneObjectTriangle::SceneObjectTriangle(Material *material, const Vector3& v0, const Vector3& v1, const Vector3& v2):
	SceneObject(material),
	v0(v0), v1(v1), v2(v2)
{
	u = v1 - v0;
	unormalized = u.normalized();
    v = v2 - v0;
	vnormalized = v.normalized();
	m_normal = u.cross(v);
	if(m_normal == Vector3::zero)
		throw invalid_argument("Triangle is degenerate");
	
	uu = u * u;
    uv = u * v;
    vv = v * v;
	uv2_uuvv = uv * uv - uu * vv;

	area = m_normal.length() / 2.0f;

	float minx = min<float>(v0.x(), min<float>(v1.x(), v2.x()));
	float miny = min<float>(v0.y(), min<float>(v1.y(), v2.y()));
	float minz = min<float>(v0.z(), min<float>(v1.z(), v2.z()));
	
	float maxx = max<float>(v0.x(), max<float>(v1.x(), v2.x()));
	float maxy = max<float>(v0.y(), max<float>(v1.y(), v2.y()));
	float maxz = max<float>(v0.z(), max<float>(v1.z(), v2.z()));

	m_aabb = AABB(minx, miny, minz, maxx, maxy, maxz);
}

Intersection SceneObjectTriangle::intersection(const Ray& ray)
{
    Vector3    dir, w0, w;           // ray vectors
    float      r, a, b;              // params to calc ray-plane intersect

	dir = ray.direction();              // ray direction vector
	w0 = ray.origin() - v0;
    a = -m_normal * w0;
    b = m_normal * dir;
    if (fabs(b) < 0.001f) {     // ray is  parallel to triangle plane
        if (a == 0)             // ray lies in triangle plane
            return Intersection(this, 0);
        else
			return Intersection::noIntersection;   // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
		return Intersection::noIntersection;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

	Vector3 I = ray.origin() + r * dir;            // intersect point of ray and plane

    // is I inside T?
    float wu, wv;
    w = I - v0;
    wu = w * u;
    wv = w * v;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / uv2_uuvv;
    if (s < 0.0 || s > 1.0)         // I is outside T
		return Intersection::noIntersection;
    t = (uv * wu - uu * wv) / uv2_uuvv;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return Intersection::noIntersection;

    return Intersection(this, r);
}

Vector3 SceneObjectTriangle::normalAt(const Ray&, const Vector3&)
{
	return m_normal;
}


Vector2 SceneObjectTriangle::textureCoordinatesAt(const Vector3& point)
{
	Vector3 AP = point - v0;
	Vector3 APxAC = AP.cross(v);
	Vector3 APxAB = AP.cross(u);

	float texCoordX = (APxAC.length() / 2.0f) / area;
	float texCoordY = (APxAB.length() / 2.0f) / area;

	return Vector2(texCoordX, texCoordY);
}

Vector3 SceneObjectTriangle::xTextureVector(const Vector3&)
{
	return unormalized;
}

Vector3 SceneObjectTriangle::yTextureVector(const Vector3&)
{
	return vnormalized;
}

AABB SceneObjectTriangle::aabb()
{
	return m_aabb;
}

Vector3 SceneObjectTriangle::a() const 
{
	return v0;
}

Vector3 SceneObjectTriangle::b() const 
{
	return v1;
}

Vector3 SceneObjectTriangle::c() const 
{
	return v2;
}

Vector3 SceneObjectTriangle::normal() const
{
	return m_normal;
}