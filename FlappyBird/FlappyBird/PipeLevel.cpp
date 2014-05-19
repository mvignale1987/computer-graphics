#include "PipeLevel.h"

const float PipeLevel::firstPipeDistance = 180;
const float PipeLevel::pipeDistance = 60;
const int PipeLevel::maxActivePipes = 20;

PipeLevel::PipeLevel(Bridge& bridge):
	bridge(bridge)
{
	// estos datos se deberían leer del xml
	pipeSizes.push_back(60);
	pipeSizes.push_back(60);
	pipeSizes.push_back(50);
	pipeSizes.push_back(20);
	pipeSizes.push_back(40);
	pipeSizes.push_back(35);
	pipeSizes.push_back(10);
	pipeSizes.push_back(20);
	pipeSizes.push_back(20);

	reset();
}

void PipeLevel::stop()
{
	for(std::deque<Pipe>::iterator it = activePipes.begin(); it != activePipes.end(); ++it)
	{
		it->stop();
	}
}

void PipeLevel::reset()
{
	nextPendingPipe = 0;
	activePipes.clear();
	addActivePipes();
}

void PipeLevel::addActivePipes()
{
	while(activePipes.size() >= maxActivePipes)
	{
		activePipes.pop_back();
	}
	while(activePipes.size() < maxActivePipes)
	{
		float pipePosition;
		if(activePipes.empty())
		{
			pipePosition = firstPipeDistance;
		} else {
			pipePosition = activePipes.front().getAbsolutePosition() + pipeDistance;
		}
		activePipes.push_front(Pipe(bridge, pipePosition, pipeSizes.at(nextPendingPipe)));
		nextPendingPipe = (nextPendingPipe + 1) % pipeSizes.size();
	}
}

void PipeLevel::render(Scene &parent)
{
	for(std::deque<Pipe>::iterator it = activePipes.begin(); it != activePipes.end(); ++it)
	{
		it->render(parent);
	}
	
	if(!activePipes.empty() && activePipes.back().getAbsolutePosition() < -firstPipeDistance*3)
	{
		activePipes.pop_back();
		addActivePipes();
	}
}