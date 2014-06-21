#pragma once

#include "Vector3.h"

class Camera
{
private:
	Vector3 pos;
	Vector3 lookAtPos;
	Vector3 upDirection;
	float	zNearVal;
	float	zFarVal;
	float	fovVal;
public:
	Camera();
	Camera(const Vector3 &pos, const Vector3 &lookAt, const Vector3 &up, float zNear, float zFar, float fov);

	Vector3 position() const;
	Vector3 lookAt() const;
	Vector3 up() const;
	float zNear() const;
	float zFar() const;
	float fov() const;
};