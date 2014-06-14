#include "Light.h"


Light::Light()
{
}

Light::Light(
	const std::string id,
	const Vector3& position,
	const Vector3& ambientColor,
	const Vector3& diffuseColor,
	float linearAttenuation,
	float quadAttenuation
):
	m_id(id),
	m_position(position),
	m_ambientColor(ambientColor),
	m_diffuseColor(diffuseColor),
	m_linearAttenuation(linearAttenuation),
	m_quadAttenuation(quadAttenuation)
{
}

std::string Light::id() const
{
	return m_id;
}

Vector3 Light::position() const
{
	return m_position;
}

Vector3 Light::ambientColor() const
{
	return m_ambientColor;
}

Vector3 Light::diffuseColor() const
{
	return m_diffuseColor;
}

float Light::linearAttenuation() const
{
	return m_linearAttenuation;
}

float Light::quadAttenuation() const
{
	return m_quadAttenuation;
}

