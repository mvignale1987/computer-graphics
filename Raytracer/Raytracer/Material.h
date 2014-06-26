#pragma once

#include "Vector3.h"
#include <string>

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
	unsigned char	m_specularExponent;
	bool			m_mirrored;
	bool			m_refractive;
	float			m_refractionIndex;
	float			m_transparency;
public:
	Material();
	Material(
		std::string	id,
		Vector3	ambientColor,
		float ambientCoefficient,
		Vector3	diffuseColor,
		float diffuseCoefficient,
		Vector3 specularColor,
		float specularCoefficient,
		int specularExponent,
		bool mirrored,
		bool refractive,
		float refractionIndex,
		float transparency
		);

	std::string	id() const;
	Vector3	ambientColor() const;
	float ambientCoefficient() const;
	Vector3	diffuseColor() const;
	float diffuseCoefficient() const;
	Vector3 specularColor() const;
	float specularCoefficient() const;
	unsigned char specularExponent() const;
	bool mirrored() const;
	bool refractive() const;
	float refractionIndex() const;
	float transparency() const;
};

