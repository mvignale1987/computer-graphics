#include "Camera.h"
#include <iostream>

const float Camera::minLatitude = 1.1f;
const float Camera::maxLatitude = 1.9f;
const float Camera::minDistance = 10.0f;
const float Camera::maxDistance = 500.0f;
const float Camera::minAzimut = (float) -M_PI * 0.25f;
const float Camera::maxAzimut = (float) M_PI * 0.25f;

Camera::Camera(float azimut, float latitude, float distance, const Vector3& center):
	azimut(azimut),
	latitude(latitude),
	distance(distance),
	center(center)
{
	recalculatePosition();
}

const Vector3& Camera::getPosition() const
{
	return position;
}

const Vector3& Camera::getCenter() const
{
	return center;
}

void Camera::handleEvent(Scene &, const SDL_Event& ev)
{
	switch(ev.type)
	{
	case SDL_MOUSEMOTION:
	{
		if((ev.motion.state & SDL_BUTTON_RMASK) == 0)
			break;
		float deltaY = ev.motion.yrel * 0.005f;
		float deltaX = ev.motion.xrel * 0.005f;
		azimut += deltaX;
		latitude += deltaY;
		recalculatePosition();
		break;
	}
	case SDL_MOUSEWHEEL:
	{
		distance -= ev.wheel.y * 10;
		recalculatePosition();
		break;
	}
	case SDL_KEYDOWN:
	{
		if(ev.key.keysym.scancode == SDL_SCANCODE_W)
			center += Vector3::up * 1;
		else if(ev.key.keysym.scancode == SDL_SCANCODE_S)
			center -= Vector3::up * 1;
		else if(ev.key.keysym.scancode == SDL_SCANCODE_A)
			center -= Vector3::right * 1;
		else if(ev.key.keysym.scancode == SDL_SCANCODE_D)
			center += Vector3::right * 1;
		else
			break;
		recalculatePosition();
	}
	default:
		break;
	}
}


void Camera::render(Scene &)
{
}

void Camera::recalculatePosition()
{
	if(latitude > maxLatitude)
		latitude = maxLatitude;
	if(latitude < minLatitude)
		latitude = minLatitude;
	if(distance < minDistance)
		distance = minDistance;
	if(distance > maxDistance)
		distance = maxDistance;
	if(azimut < minAzimut)
		azimut = minAzimut;
	if(azimut > maxAzimut)
		azimut = maxAzimut;

	position = center + Vector3(
			distance * sinf(latitude) * sinf(azimut), // x
			distance * cosf(latitude), // y
			distance * sinf(latitude) * cosf(azimut) // z
			);

	//std::cout << "Azimut: " << azimut << ", Latitude: " << latitude << ", distance: " << distance << ", Position: " << center << std::endl;
}