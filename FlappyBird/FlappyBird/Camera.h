#pragma once

#include "SceneObject.h"
#include "Vector3.h"

class Camera: public SceneObject
{
private:
	static const float minLatitude, maxLatitude;
	float	distance, latitude, azimut;
	Vector3 position;
public:
	Camera(const Vector3& startingPosition = Vector3::backward * 4);
	const Vector3& getPosition() const;
	float x();
	float y();
	float z();
	void handleEvent(Scene &parent, const SDL_Event& ev);
	void render(Scene &parent);
};