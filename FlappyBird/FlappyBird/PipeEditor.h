#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Bridge.h"
#include "Flappy.h"


class PipeEditor: public SceneObject
{
private:
	static const float aperture;
	static const float upperPipeLength;
	static const float ratio;
	static const int  slices;
	static const int stacks;

	float apertureHeight; // y position of aperture
	float position; // x position relative to flappy
	Bridge &colliderBridge;
	bool stopped;
public:
	PipeEditor(Bridge& colliderBridge, float initialPosition, float apertureHeight);

	float getAbsolutePosition() const;
	void render(Scene &parent);

};