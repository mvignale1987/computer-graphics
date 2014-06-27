#include "SceneObjectQuad.h"

using namespace std;

SceneObjectQuad::SceneObjectQuad(Material *material, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d):
	SceneObject(material)
{
	m_vertexs.push_back(a);
	m_vertexs.push_back(b);
	m_vertexs.push_back(c);
	m_vertexs.push_back(d);

	v0 = a;
	u = (b - a).normalized();
	v = (d - a).normalized();
	lengthU = (b - a).length();
	lengthV = (d - a).length();
	Vector3 w = c - a;
	m_normal = u.cross(v).normalized();
	if((u.cross(w).normalized() - m_normal).length() > 0.1)
		throw std::invalid_argument("Points are not coplanar");

	float minx = min<float>(a.x(), min<float>(b.x(), min<float>(c.x(), d.x())));
	float miny = min<float>(a.y(), min<float>(b.y(), min<float>(c.y(), d.x())));
	float minz = min<float>(a.z(), min<float>(b.z(), min<float>(c.z(), d.z())));
	
	float maxx = max<float>(a.x(), max<float>(b.x(), max<float>(c.x(), d.x())));
	float maxy = max<float>(a.y(), max<float>(b.y(), max<float>(c.y(), d.y())));
	float maxz = max<float>(a.z(), max<float>(b.z(), max<float>(c.z(), d.z())));

	m_aabb = AABB(minx, miny, minz, maxx, maxy, maxz);
}


Intersection SceneObjectQuad::intersection(const Ray& r)
{
	Vector3 origin = r.origin();
	Vector3 direction = r.direction();
	float distance;
	float numerator = (m_vertexs[0] - origin) * m_normal;
	float denominator = direction * m_normal;
	if(numerator == 0)
		distance = 0; // r.origin lies on the plane
	else if(denominator == 0)
		return Intersection::noIntersection; // ray is parallel 
	else
		distance = numerator / denominator; // ray intersects on single place
	
	if(distance <= 0.001f || distance > 1000.0f)
	{
		return Intersection::noIntersection;
	}

	// calc angle between point and m_vertexs
	float anglesum=0;
	Vector3 q = r.origin() + distance * r.direction();
	int n = m_vertexs.size();
	for(int i = 0; i < n; i++) {
		Vector3 p1 = m_vertexs[i] - q;
		Vector3 p2 = m_vertexs[(i+1)%n] - q;

		float m1 = p1.length();
		float m2 = p2.length();
		if (m1*m2 <= 0.001f)
			return Intersection(this, distance); /* We are on a node, consider this inside */
		else {
			float costheta = (p1*p2) / (m1*m2);
			anglesum += acosf(costheta);
		}
	}

	if(fabs(anglesum - 2 * M_PI) < 0.01f)
		return Intersection(this, distance);
	else
		return Intersection::noIntersection;
}

Vector3 SceneObjectQuad::normalAt(const Ray&, const Vector3&)
{
	return m_normal;
}

AABB SceneObjectQuad::aabb()
{
	return m_aabb;
}

Vector3 SceneObjectQuad::normal()
{
	return m_normal;
}

Vector2 SceneObjectQuad::textureCoordinatesAt(const Vector3& point)
{
	Vector3 p = point - v0;
	return Vector2((p * v) / lengthV, (p * u) / lengthU);
}

Vector3 SceneObjectQuad::xTextureVector(const Vector3&)
{
	return v;
}

Vector3 SceneObjectQuad::yTextureVector(const Vector3&)
{
	return u;
}

std::vector<Vector3>& SceneObjectQuad::vertexs()
{
	return m_vertexs;
}