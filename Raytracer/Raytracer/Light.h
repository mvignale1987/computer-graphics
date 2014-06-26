#pragma once

#include "Vector3.h"
#include <string>

class Light
{
private:
	Vector3 m_position;
	Vector3 m_ambientColor;
	Vector3 m_diffuseColor;
	float m_linearAttenuation;
	float m_quadAttenuation;
public:
	Light();
	Light(
		const Vector3& position,
		const Vector3& ambientColor,
		const Vector3& diffuseColor,
		float linearAttenuation,
		float quadAttenuation
	);

	Vector3 position() const;
	Vector3 ambientColor() const;
	Vector3 diffuseColor() const;
	float linearAttenuation() const;
	float quadAttenuation() const;
};

