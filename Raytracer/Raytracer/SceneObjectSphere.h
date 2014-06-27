#pragma once

#include "SceneObject.h"

class SceneObjectSphere: public SceneObject
{
private:
	Vector3 center;
	float radius;
public:
	SceneObjectSphere(Material *material, const Vector3& center, float radius);

	Vector3 position() const;
	float sphereRadius() const;

	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& p);
	virtual Vector2 textureCoordinatesAt(const Vector3& point);
};

