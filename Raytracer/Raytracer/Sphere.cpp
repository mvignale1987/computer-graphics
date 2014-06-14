#include "Sphere.h"

Sphere::Sphere():
	radius(1)
{
}

Sphere::Sphere(float r):
	radius(r)
{
}
float Sphere::getRadius(){
	return radius;

}
