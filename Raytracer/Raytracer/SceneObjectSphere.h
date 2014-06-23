#pragma once

#include "SceneObject.h"

class SceneObjectSphere: public SceneObject
{
private:
	Vector3 center;
	float radius;
public:
	SceneObjectSphere(Material *material, const Vector3& center, float radius);

	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& p);
};

