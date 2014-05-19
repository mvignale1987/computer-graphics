#pragma once

#include "SceneObject.h"
#include "Pipe.h"
#include "Bridge.h"
#include <vector>
#include <deque>

class PipeLevel: public SceneObject
{
private:
	const static float firstPipeDistance;
	const static float pipeDistance;
	const static int   maxActivePipes;

	Bridge &			bridge;
	std::vector<float>	pipeSizes;
	std::deque<Pipe>	activePipes;
	int					nextPendingPipe;
public:
	PipeLevel(Bridge& bridge);

	void render(Scene &parent);
	void stop();
	void reset();
private:
	void addActivePipes();
};