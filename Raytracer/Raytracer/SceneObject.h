#pragma once

#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Vector2.h"
#include "AABB.h"

class SceneObject
{
private:
	Material *m_material;
protected:
	SceneObject(
		Material *material
	);
public:
	// Object associated material
	Material *material() const;
	// Computes a potential intersection from a ray
	virtual Intersection intersection(const Ray& r) = 0;
	// Object normal at certain point (seen from a ray)
	virtual Vector3 normalAt(const Ray& r, const Vector3& point) = 0;
	// Texture coordinates from a certain point
	virtual Vector2 textureCoordinatesAt(const Vector3& point) = 0;
	// u vector in uv coordinates
	virtual Vector3 xTextureVector(const Vector3& point) = 0;
	// v vector in uv coordinates
	virtual Vector3 yTextureVector(const Vector3& point) = 0;
	// AABB containing object
	virtual AABB aabb() = 0;
};