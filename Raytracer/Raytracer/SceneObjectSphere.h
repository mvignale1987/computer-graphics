#pragma once

#include "SceneObject.h"

class SceneObjectSphere: public SceneObject
{
private:
	Vector3 m_center;
	float	m_radius;
	AABB	m_aabb;
public:
	SceneObjectSphere(Material *material, const Vector3& center, float radius);

	Vector3 center() const;
	float radius() const;

	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& p);
	virtual Vector2 textureCoordinatesAt(const Vector3& point);
	virtual Vector3 xTextureVector(const Vector3& point);
	virtual Vector3 yTextureVector(const Vector3& point);
	virtual AABB aabb();
};

