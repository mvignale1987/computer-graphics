#pragma once

#include "SceneObject.h"
#include "Model.h"
#include "Camera.h"

class Bridge: public SceneObject
{
private: 
	static const int nBridges;
	static const float modelLength;

	Model model;
	Camera *camera;
	float animTime;
	GLuint texturedDisplayList, solidDisplayList, wireframeDisplayList;
public:
	Bridge(Camera *);
	void render(Scene &parent);
	void render(RenderMode mode);
};