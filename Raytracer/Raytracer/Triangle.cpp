#include "Triangle.h"


Triangle::Triangle():
	ShapeDefinition(Shape::TRIANGLE, "")
{
	m_points[0] = m_points[1] = m_points[2] = Vector3::zero;
}

Triangle::Triangle(const std::string& id,
		const Vector3 &p1,
		const Vector3 &p2,
		const Vector3 &p3
		):
	ShapeDefinition(Shape::TRIANGLE, id)
{
	m_points[0] = p1;
	m_points[1] = p2;
	m_points[2] = p3;
}

std::vector<Vector3> Triangle::points() const
{
	std::vector<Vector3> res(3);
	res[0] = m_points[0];
	res[1] = m_points[1];
	res[2] = m_points[2];
	return res;
}

const Vector3 *Triangle::pointsPointer() const
{
	return m_points;
}