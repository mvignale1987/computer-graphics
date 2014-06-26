#include "Material.h"
#include <FreeImage.h>

Material::Material():
	m_id(""),
	m_ambientColor(Vector3::zero),
	m_ambientCoefficient(0),
	m_diffuseColor(Vector3::zero),
	m_diffuseCoefficient(0),
	m_specularColor(Vector3::one),
	m_specularCoefficient(0),
	m_specularExponent(0),
	m_mirrored(false),
	m_refractive(false),
	m_refractionIndex(1),
	m_transparency(1),
	texture(NULL)
{
}

Material::Material(
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
	std::string texturePath
):
	m_id(id),
	m_ambientColor(ambientColor),
	m_ambientCoefficient(ambientCoefficient),
	m_diffuseColor(diffuseColor),
	m_diffuseCoefficient(diffuseCoefficient),
	m_specularColor(specularColor),
	m_specularCoefficient(specularCoefficient),
	m_specularExponent((unsigned char)specularExponent),
	m_mirrored(mirrored),
	m_refractive(refractive),
	m_refractionIndex(refractionIndex),
	m_transparency(transparency)
{
	if(ambientCoefficient < 0 || ambientCoefficient > 1)
		throw std::out_of_range("ambientCoefficient must be betweeen 0 and 1");
	if(diffuseCoefficient < 0 || diffuseCoefficient > 1)
		throw std::out_of_range("diffuseCoefficient must be betweeen 0 and 1");
	if(specularCoefficient < 0 || specularCoefficient > 1)
		throw std::out_of_range("specularCoefficient must be betweeen 0 and 1");
	if(specularExponent < 0 || specularExponent > 255)
		throw std::out_of_range("specularCoefficient must be betweeen 0 and 255");
	if(refractionIndex <= 0 )
		throw std::out_of_range("refraction index must be positive");
	if(transparency < 0 || transparency > 1)
		throw std::out_of_range("transparency must be betweeen 0 and 1");

	if(!texturePath.empty())
	{
		const char *filename = texturePath.c_str();
		texture = NULL;
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		// check the file signature and deduce its format
		// (the second argument is currently not used by FreeImage)
		fif = FreeImage_GetFileType(filename, 0);
		if(fif == FIF_UNKNOWN) {
			// no signature ?
			// try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(filename);
		}
		// check that the plugin has reading capabilities ...
		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
			// ok, let's load the file
			texture = FreeImage_Load(fif, filename);
			// unless a bad file format, we are done !
		}

		if(texture == NULL)
			throw std::invalid_argument("Couldn't load texture '" + texturePath + "'");

		textureWidth = FreeImage_GetWidth(texture);
		textureHeight = FreeImage_GetHeight(texture);
	}
}

std::string	Material::id() const
{
	return m_id;
}
Vector3	Material::ambientColor() const
{
	return m_ambientColor;
}

float Material::ambientCoefficient() const
{
	return m_ambientCoefficient;
}

Vector3	Material::diffuseColor() const
{
	return m_diffuseColor;
}

float Material::diffuseCoefficient() const
{
	return m_diffuseCoefficient;
}

Vector3 Material::specularColor() const
{
	return m_specularColor;
}

float Material::specularCoefficient() const
{
	return m_specularCoefficient;
}

unsigned char Material::specularExponent() const
{
	return m_specularExponent;
}

bool Material::mirrored() const
{
	return m_mirrored;
}

bool Material::refractive() const
{
	return m_refractive;
}

float Material::refractionIndex() const
{
	return m_refractionIndex;
}

float Material::transparency() const
{
	return m_transparency;
}

bool Material::textured() const 
{
	return texture != NULL;
}

Vector3 Material::getColorAt(const Vector2& uv) const
{
	unsigned x = (unsigned) ceilf(uv.x() * textureWidth);
	unsigned y = (unsigned) ceilf(uv.y() * textureHeight);

	RGBQUAD res;

	if(x >= textureWidth)
		x = textureWidth - 1;
	if(y >= textureHeight)
		y = textureHeight - 1;

	if(!FreeImage_GetPixelColor(texture, x, y, &res))
	{
		return Vector3::zero;
	} else
	{
		float r = res.rgbRed / 255.0f;
		float g = res.rgbGreen / 255.0f;
		float b = res.rgbBlue / 255.0f;
		return Vector3(r, g, b);
	}
}