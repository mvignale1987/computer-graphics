#pragma once

#include "Vector3.h"

class SceneObject;

class Intersection
{
public:
	// intersección a distancia infinita
	static const Intersection noIntersection;
	static const float minIntersectionDistance;
private:
	Vector3 m_normal;
	Vector3 m_point;
	float m_distance;
	SceneObject *m_object;
public:
	Intersection();
	Intersection(SceneObject *obj, const Vector3& point, const Vector3& normal, float distance);

	SceneObject *obj() const;
	Vector3 normal() const;
	Vector3 point() const;
	float distance() const;

	bool intersects() const;

	operator bool() const;
};

