#pragma once

#include "Material.h"
#include "Intersection.h"
#include "Ray.h"

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
};