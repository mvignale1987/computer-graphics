#include "Material.h"


Material::Material():
	m_ambientCoefficient(1),
	m_diffuseCoefficient(1),
	m_specularCoefficient(1),
	m_specularExponent(1)
{
}

Material::Material(
	const std::string& id,
	const Vector3& ambientColor,
	float ambientCoefficient,
	const Vector3& diffuseColor,
	float diffuseCoefficient,
	const Vector3& specularColor,
	float specularCoefficient,
	unsigned int specularExponent
	):
	m_id(id),
	m_ambientColor(ambientColor),
	m_ambientCoefficient(ambientCoefficient),
	m_diffuseColor(diffuseColor),
	m_diffuseCoefficient(diffuseCoefficient),
	m_specularColor(specularColor),
	m_specularCoefficient(specularCoefficient),
	m_specularExponent(specularExponent)
{
}

std::string Material::id() const
{
	return m_id;
}

Vector3 Material::ambientColor() const
{
	return m_ambientColor;
}

float Material::ambientCoefficient() const
{
	return m_ambientCoefficient;
}

Vector3 Material::diffuseColor() const
{
	return m_diffuseColor;
}

float Material::diffuseCoefficient() const
{
	return m_diffuseCoefficient;
}

Vector3	Material::specularColor() const
{
	return m_specularColor;
}

float Material::specularCoefficient() const
{
	return m_specularCoefficient;
}

unsigned int Material::specularExponent() const
{
	return m_specularExponent;
}
