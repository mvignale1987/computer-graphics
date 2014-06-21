#pragma once
#include <string>
#include "Shape.h"

class Sphere;
class Cyllinder;
class Quad;
class Triangle;

class ShapeDefinition
{
private:
	std::string m_id;
	Shape::Shape m_shapeType;
protected:
	ShapeDefinition(Shape::Shape shapeType, const std::string& id);
public:
	std::string id() const;

	Shape::Shape shapeType() const;

	Sphere& asSphere();
	Cyllinder& asCyllinder();
	Quad& asQuad();
	Triangle& asTriangle();

	virtual ~ShapeDefinition();
};

