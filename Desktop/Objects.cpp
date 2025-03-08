#include "Objects.h"
#include <math.h>

Objects::Objects(Vector2f center, float radius, int id) :_radius(radius), id(id), Center(center)
{

}

Objects::Objects(){}

Objects::~Objects() = default;

void Objects::setRadius(float newRadius)
{
	_radius = newRadius;
}

float Objects::getRadius() const
{
	return _radius;
}

Vector2f Objects::getCenter() const
{
	return Center;
}

void Objects::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}

void Objects::TimeElapsed(int diff)
{

}

int Objects::getID()
{
	return id;
}