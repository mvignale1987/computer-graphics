#include "Options.h"

Options::Options():
	textures(true),
	smoothShading(true),
	wireframe(false),
	gameSpeed(MEDIUM)
{
}

bool Options::getTexturesEnabled() const
{
	return textures;
}

void Options::setTexturesEnabled(bool val)
{
	textures = val;
}

bool Options::getSmoothShading() const
{
	return smoothShading;
}

void Options::setSmoothShading(bool val)
{
	smoothShading = val;
}

bool Options::getWireframeEnabled() const
{
	return wireframe;
}

void Options::setWireframeEnabled(bool val)
{
	wireframe = val;
}

GameSpeed Options::getGameSpeed() const
{
	return gameSpeed;
}

float Options::speedMultiplier() const
{
	switch (gameSpeed)
	{
	case SLOW:
		return 0.75f;
	case MEDIUM:
		return 1.0f;
	case FAST:
		return 1.25f;
	default:
		return 1.0f;
	}
}

void Options::setGameSpeed(GameSpeed val)
{
	gameSpeed = val;
}

void glToggle(GLenum cap, GLboolean val)
{
	(val ? glEnable : glDisable)(cap);
}