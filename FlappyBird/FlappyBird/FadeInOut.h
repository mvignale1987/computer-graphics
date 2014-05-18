#pragma once

#include "SceneObject.h"

class FadeInOut: public SceneObject
{
private:
	static const float length;
	float animTime;
public:
	FadeInOut();
	void render(Scene &parent);
	bool fadedOut() const;
};
