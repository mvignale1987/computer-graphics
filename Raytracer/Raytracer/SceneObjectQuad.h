#pragma once
#include "SceneObject.h"
#include <vector>

class SceneObjectQuad : public SceneObject
{
private:
	std::vector<Vector3> vertexs;
	// para mapeo de texturas
	Vector3 v0;
	Vector3 u, v; 
	float lengthU, lengthV;
	Vector3 normal;
public:
	SceneObjectQuad(Material *material, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);
	virtual Intersection intersection(const Ray& r);
	virtual Vector3 normalAt(const Ray& r, const Vector3& point);
	virtual Vector2 textureCoordinatesAt(const Vector3& point);
};

