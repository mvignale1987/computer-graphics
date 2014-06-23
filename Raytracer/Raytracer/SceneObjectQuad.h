#pragma once
#include "SceneObject.h"
#include <vector>

class SceneObjectQuad : public SceneObject
{
private:
	std::vector<Vector3> vertexs;
	Vector3 normal;
public:
	SceneObjectQuad(Material *material, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);
	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& point);
};

