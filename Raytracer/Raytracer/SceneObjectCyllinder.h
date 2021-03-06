#pragma once

#include "SceneObject.h"

class SceneObjectCyllinder: public SceneObject
{
private:
	Vector3 m_center;
	float	m_radius;
	float	m_height;
	AABB	m_aabb;
public:
	SceneObjectCyllinder(Material *material, const Vector3& center, float radius, float height);

	Vector3 center() const;
	float   radius() const;
	float   height() const;

	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& point);
	virtual Vector2 textureCoordinatesAt(const Vector3& point);
	virtual Vector3 xTextureVector(const Vector3& point);
	virtual Vector3 yTextureVector(const Vector3& point);
	virtual AABB aabb();
};

