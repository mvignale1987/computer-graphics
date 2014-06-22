#pragma once

#include "Vector3.h"
#include "ShapeDefinition.h"
#include <vector>

class Quad: public ShapeDefinition
{
private:
	Vector3 m_points[4];
public:
	Quad();
	Quad(
		const std::string& id,
		const Vector3 &p1,
		const Vector3 &p2,
		const Vector3 &p3,
		const Vector3 &p4
		);

	std::vector<Vector3> points() const;
	const Vector3 *pointsPointer() const;

	Intersection intersection(const Ray& ray);
};

