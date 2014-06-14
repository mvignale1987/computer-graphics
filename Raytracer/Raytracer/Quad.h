#pragma once

#include "Vector3.h"
#include <vector>

class Quad
{
private:
	Vector3 m_points[4];
public:
	Quad();
	Quad(const Vector3 &p1,
		const Vector3 &p2,
		const Vector3 &p3,
		const Vector3 &p4
		);

	std::vector<Vector3> points() const;
	const Vector3 *pointsPointer() const;
};

