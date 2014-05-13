#include "Camera.h"

const float Camera::minLatitude = M_PI / 2 * 0.1;
const float Camera::maxLatitude = M_PI / 2;

Camera::Camera(const Vector3& startingPosition):
	position(startingPosition)
{
	distance = position.length();
	float x = position.x();
	float y = position.y();
	float z = position.z();
	float pi = (float)M_PI;

	// converts from cartesian coordinates to polar coordinates
	// TODO fix this transformation :p
	if(y == 0)
		latitude = pi / 2;
	else if(y > 0)
		latitude = pi / 2 + atanf(sqrtf(x*x + y*y)/-z);
	else
		latitude = pi +  atanf(sqrtf(x*x + y*y)/-z);

	if(latitude > maxLatitude)
		latitude = maxLatitude;
	if(latitude < minLatitude)
		latitude = minLatitude;

	if(z > 0 && x > 0)
		azimut = atanf(x/z);
	else if(z > 0 && x < 0)
		azimut = 2 * pi + atanf(x/z);
	else if(z == 0 && x > 0)
		azimut = pi / 2;
	else if(z == 0 && x < 0)
		azimut = - pi / 2;
	else if(z == 0 && x == 0)
		azimut = 0;
	else
		azimut = pi + atanf(x/z);
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

	if(latitude > maxLatitude)
		latitude = maxLatitude;
	if(latitude < minLatitude)
		latitude = minLatitude;
	
	position = Vector3(
		distance * sinf(latitude) * sinf(azimut), // x
		distance * cosf(latitude),
		distance * sinf(latitude) * cosf(azimut) // z
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