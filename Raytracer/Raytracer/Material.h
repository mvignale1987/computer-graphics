#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include <string>

struct FIBITMAP;

class Material
{
private:
	std::string		m_id;
	
	// lighting 
	Vector3			m_ambientColor;
	float			m_ambientCoefficient;
	Vector3			m_diffuseColor;
	float			m_diffuseCoefficient;
	Vector3			m_specularColor;
	float			m_specularCoefficient;
	unsigned char	m_specularExponent;

	// mirror & refractive 
	bool			m_mirrored;
	bool			m_refractive;
	float			m_refractionIndex;
	float			m_transparency;
	
	// texture 
	FIBITMAP *		texture;
	unsigned int	textureWidth;
	unsigned int	textureHeight;

	// normal map
	FIBITMAP *		normalMap;
	unsigned int	normalMapWidth;
	unsigned int	normalMapHeight;
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
		float transparency,
		std::string texturePath,
		std::string normalMapPath
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
	bool textured() const;
	Vector3 getColorAt(const Vector2& uv) const;
	bool hasNormalMap() const;
	Vector3 getNormalAt(const Vector2& uv) const;

private:
	static void loadTexture(const std::string& path, FIBITMAP *& bitmap, unsigned int& width, unsigned int& height);
};

