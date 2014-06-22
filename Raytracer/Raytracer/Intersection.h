#pragma once

#include "Vector3.h"

class SceneObject;

class Intersection
{
public:
	// intersección a distancia infinita
	static const Intersection noIntersection;
private:
	float m_distance;
	SceneObject *m_object;
public:
	Intersection();
	Intersection(SceneObject *obj, float distance);

	SceneObject *obj() const;
	float distance() const;

	bool intersects() const;

	operator bool() const;
};

