#include "Camera.h"

Camera::Camera(const Vector3& startingPosition):
	position(startingPosition)
{
	distance = position.length();
	float x = position.x();
	float y = position.y();
	float z = position.z();
	float pi = (float)M_PI;

	// converts from cartesian coordinates to polar coordinates
	if(z == 0)
		latitude = pi / 2;
	else if(z > 0)
		latitude = atanf(sqrtf(x*x + y*y)/z);
	else
		latitude = pi +  atanf(sqrtf(x*x + y*y)/z);

	if(x > 0 && y > 0)
		azimut = atanf(y/x);
	else if(x > 0 && y < 0)
		azimut = 2 * pi + atanf(y/x);
	else if(x == 0 && y > 0)
		azimut = pi / 2;
	else if(x == 0 && y < 0)
		azimut = - pi / 2;
	else if(x == 0 && y == 0)
		azimut = 0;
	else
		azimut = pi + atanf(y/x);
}

const Vector3& Camera::getPosition() const
{
	return position;
}

void Camera::handleEvent(Scene &, const SDL_Event& ev)
{
	if(ev.type != SDL_MOUSEMOTION)
		return;
	if((ev.motion.state & SDL_BUTTON_RMASK) == 0)
		return;

	float deltaY = ev.motion.yrel * 0.005f;
	float deltaX = ev.motion.xrel * 0.005f;
	azimut += deltaX;
	latitude += deltaY;

	position = Vector3(
		distance * sinf(azimut) * cosf(latitude),
		distance * sinf(azimut) * sinf(latitude),
		distance * cosf(azimut)
		);
}

float Camera::x()
{
	return position.x();
}

float Camera::y()
{
	return position.y();
}

float Camera::z()
{
	return position.z();
}

void Camera::render(Scene &parent)
{
}