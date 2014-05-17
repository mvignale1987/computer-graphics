#pragma once

#include "SceneObject.h"
#include "Model.h"
#include "Camera.h"

class Bridge: public SceneObject
{
private: 
	static const int nBridges;
	static const float modelLength;
	static const float bridgeVelocity;

	Model model;
	float animTime;
	GLuint texturedDisplayList, solidDisplayList, wireframeDisplayList;
public:
	Bridge();
	void render(Scene &parent);
	void render(RenderMode mode);
	float getStreetHeight(); // retorna la altura de la calle en el momento actual
};