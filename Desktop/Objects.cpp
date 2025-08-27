#include "Objects.h"
#include <math.h>



Objects::Objects(Vector2f center, float radius, string id) :_radius(radius), id(id), Center(center), m_IsLive(true), m_isMarked(false), m_mass(0.f)
{
}

void Objects::TimeElapsed(int diff)
{
}

Objects::~Objects() = default;

void Objects::setRadius(float newRadius)
{
	_radius = newRadius;
}

void Objects::setMass(const float mass)
{
	m_mass = mass;
}

float Objects::getMass() const
{
	return m_mass;
}

float Objects::getRadius() const
{
	return _radius;
}

Vector2f Objects::getCenter() const
{
	return Center;
}



string Objects::getID() const
{
	return id;
}