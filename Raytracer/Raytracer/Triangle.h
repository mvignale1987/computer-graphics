#pragma once

#include "Vector3.h"
#include "ShapeDefinition.h"
#include <vector>

class Triangle: public ShapeDefinition
{
private:
	Vector3 m_points[3];
public:
	Triangle();
	Triangle(const Vector3 &p1,
		const Vector3 &p2,
		const Vector3 &p3
		);

	std::vector<Vector3> points() const;
	const Vector3 *pointsPointer() const;
};