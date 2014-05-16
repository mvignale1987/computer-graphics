#pragma once

#include "SceneObject.h"
#include "Model.h"
#include "Camera.h"

class Bridge: public SceneObject
{
private: 
	Model model;
	Camera *camera;
public:
	Bridge(Camera *);
	void render(Scene &parent);
};