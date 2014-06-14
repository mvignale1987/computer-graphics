#pragma once

#include <string>
#include "Vector3.h"

class Material
{
private:
	std::string		m_id;
	Vector3			m_ambientColor;
	float			m_ambientCoefficient;
	Vector3			m_diffuseColor;
	float			m_diffuseCoefficient;
	Vector3			m_specularColor;
	float			m_specularCoefficient;
	unsigned int	m_specularExponent;
public:
	Material();
	Material(
		const std::string& id,
		const Vector3& ambientColor,
		float ambientCoefficient,
		const Vector3& diffuseColor,
		float diffuseCoefficient,
		const Vector3& specularColor,
		float specularCoefficient,
		unsigned int specularExponent
		);

	std::string		id() const;
	Vector3			ambientColor() const;
	float			ambientCoefficient() const;
	Vector3			diffuseColor() const;
	float			diffuseCoefficient() const;
	Vector3			specularColor() const;
	float			specularCoefficient() const;
	unsigned int	specularExponent() const;
};

