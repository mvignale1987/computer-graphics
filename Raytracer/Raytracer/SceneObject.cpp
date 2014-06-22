#include "SceneObject.h"


SceneObject::SceneObject(
	Material *material
):
	m_material(material)
{
}


Material *SceneObject::material() const
{
	return m_material;
}