#pragma once

#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Vector2.h"

class SceneObject
{
private:
	Material *m_material;
protected:
	SceneObject(
		Material *material
	);
public:
	Material *material() const;
	virtual Intersection intersection(const Ray& r) = 0;
	virtual Vector3 normalAt(const Ray& r, const Vector3& point) = 0;
	virtual Vector2 textureCoordinatesAt(const Vector3& point) = 0;
};