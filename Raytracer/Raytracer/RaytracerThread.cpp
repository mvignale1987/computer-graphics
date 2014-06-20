#include "RaytracerThread.h"
#include "RaytracerRenderer.h"
#include "Scene.h"

RaytracerThread::RaytracerThread(RaytracerRenderer *parent, size_t bufferPosition, Vector3 direction):
	parent(parent), bufferPosition(bufferPosition)
{
	Camera camera = parent->scene().camera();
	Vector3 origin = camera.position();
	ray = Ray(origin, direction.normalized());
}