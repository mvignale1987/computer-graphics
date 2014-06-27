#pragma once

#include "SceneObject.h"

class SceneObjectTriangle : public SceneObject
{
private:
	Vector3 v0, v1, v2;
	Vector3 u, v;
	float uv, vv, uu, uv2_uuvv;
	Vector3 m_normal;
	float area;
public:
	SceneObjectTriangle(Material *material, const Vector3& v0, const Vector3& v1, const Vector3& v2);
	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& point);
	virtual Vector2 textureCoordinatesAt(const Vector3& point);

	Vector3 a() const;
	Vector3 b() const;
	Vector3 c() const;

	Vector3 normal() const;
};


