#include "ShapeDefinition.h"
#include "Sphere.h"
#include "Cyllinder.h"
#include "Quad.h"
#include "Triangle.h"

using namespace std;

ShapeDefinition::ShapeDefinition(Shape::Shape shapeType, const string& id):
	m_id(id),
	m_shapeType(shapeType)
{
}

string ShapeDefinition::id() const
{
	return m_id;
}

Shape::Shape ShapeDefinition::shapeType() const
{
	return m_shapeType;
}

Sphere& ShapeDefinition::asSphere()
{
	if(m_shapeType != Shape::SPHERE)
		throw invalid_argument("Shape is not an Sphere");
	return *(Sphere *)(this);
}

Cyllinder& ShapeDefinition::asCyllinder()
{
	if(m_shapeType != Shape::CYLLINDER)
		throw invalid_argument("Shape is not an Cyllinder");
	return *(Cyllinder *)(this);
}

Quad& ShapeDefinition::asQuad()
{
	if(m_shapeType != Shape::QUAD)
		throw invalid_argument("Shape is not an Quad");
	return *(Quad *)(this);
}

Triangle& ShapeDefinition::asTriangle()
{
	if(m_shapeType != Shape::TRIANGLE)
		throw invalid_argument("Shape is not an Triangle");
	return *(Triangle *)(this);
}

ShapeDefinition::~ShapeDefinition()
{
}
