#pragma once

#include "Ray.h"

class RaytracerRenderer;

struct RaytracerThread
{
private:
	RaytracerRenderer *parent;
	size_t bufferPosition;
	Ray ray;
public:
	RaytracerThread(RaytracerRenderer *parent, size_t bufferPosition, const Vector3 direction);
	
	static DWORD work(void *RaytracerThreadPointer);
};

