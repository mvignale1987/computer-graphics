#pragma once

#include "Material.h"
#include "ShapeDefinition.h"

class SceneObject
{
private:
	ShapeDefinition *m_shape;
	Vector3			m_position;
	Vector3			m_rotation;
	Material		*m_material;
public:
	SceneObject();

	SceneObject(
		ShapeDefinition *shape,
		Vector3 position,
		Vector3 rotation,
		Material *material
	);

	ShapeDefinition *shape() const;
	Vector3 position() const;
	Vector3 rotation() const;
	Material *material() const;
};