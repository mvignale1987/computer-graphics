#include "Camera.h"

Camera::Camera():
	pos(Vector3::zero),
	rot(Vector3::zero),
	upDirection(Vector3::up),
	zNearVal(-1),
	zFarVal(1),
	fovVal(30)
{
}

Camera::Camera(const Vector3 &pos, const Vector3 &rot, const Vector3 &up, float zNear, float zFar, float fov):
	pos(pos),
	rot(rot),
	upDirection(up),
	zNearVal(zNear),
	zFarVal(zFar),
	fovVal(fov)
{
}

Vector3 Camera::position() const
{
	return pos;
}
Vector3 Camera::rotation() const
{
	return rot;
}
Vector3 Camera::up() const
{
	return upDirection;
}
float Camera::zNear() const
{
	return zNearVal;
}
float Camera::zFar() const
{
	return zFarVal;
}
float Camera::fov() const
{
	return fovVal;
}