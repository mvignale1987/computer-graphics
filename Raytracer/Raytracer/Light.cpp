#include "Light.h"


Light::Light():
	m_linearAttenuation(0),
	m_quadAttenuation(0)
{
}

Light::Light(
	const Vector3& position,
	const Vector3& ambientColor,
	const Vector3& diffuseColor,
	float linearAttenuation,
	float quadAttenuation
):
	m_position(position),
	m_ambientColor(ambientColor),
	m_diffuseColor(diffuseColor),
	m_linearAttenuation(linearAttenuation),
	m_quadAttenuation(quadAttenuation)
{
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

