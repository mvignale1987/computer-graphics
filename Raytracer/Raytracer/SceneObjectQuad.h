#pragma once
#include "SceneObject.h"
class SceneObjectQuad : public SceneObject
{
private:
	Vector3 a, b, c, d;
	Vector3 normal;
public:
	SceneObjectQuad(Material *material, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);
	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Vector3& point);
};

