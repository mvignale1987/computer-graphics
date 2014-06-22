#include "Quad.h"


Quad::Quad():
	ShapeDefinition(Shape::QUAD, "")
{
	m_points[0] = m_points[1] = m_points[2] = m_points[3] = Vector3::zero;
}

Quad::Quad(
		const std::string& id,
		const Vector3 &p1,
		const Vector3 &p2,
		const Vector3 &p3,
		const Vector3 &p4
		):
	ShapeDefinition(Shape::QUAD, id)
{
	m_points[0] = p1;
	m_points[1] = p2;
	m_points[2] = p3;
	m_points[3] = p4;
}

std::vector<Vector3> Quad::points() const
{
	std::vector<Vector3> res(4);
	res[0] = m_points[0];
	res[1] = m_points[1];
	res[2] = m_points[2];
	res[3] = m_points[3];
	return res;
}

const Vector3 *Quad::pointsPointer() const
{
	return m_points;
}

Intersection Quad::intersection(const Ray& ray)
{
	return Intersection::noIntersection;
}