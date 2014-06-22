#include "SceneObject.h"

SceneObject::SceneObject():
	m_shape(NULL),
	m_position(Vector3::zero),
	m_rotation(Vector3::zero),
	m_material(NULL)
{
}

SceneObject::SceneObject(
	ShapeDefinition *shape,
	Vector3 position,
	Vector3 rotation,
	Material *material
):
	m_shape(shape),
	m_position(position),
	m_rotation(rotation),
	m_material(material)
{
}

ShapeDefinition *SceneObject::shape() const
{
	return m_shape;
}

Vector3 SceneObject::position() const
{
	return m_position;
}

Vector3 SceneObject::rotation() const
{
	return m_rotation;
}

Material *SceneObject::material() const
{
	return m_material;
}

Intersection SceneObject::intersection(const Ray& r)
{
	return m_shape->intersection(r);
}