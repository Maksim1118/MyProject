#include "Hero.h"
#include <iostream>
//#include "BaseOld.h"

#include "ResourсeManager.h"

Hero::Hero(Vector2f center, float radius, int Col, string text, string id): Bot(center, radius, Col, text, id)
{
	m_IsGameOver = false;
}

void Hero::TimeElapsed(int diff)
{	
	//TODO: если расстояние до мышки меньше полурадиуса, то скорость равна нулю.

	float len = GetLen(Mouse);
	float radius = getRadius();

	if (len < radius)
	{
		maxV *= (len / radius);
	}

	Vector2f d = getIdentityVector(Mouse);
	Vector2f f = calcAttractionForce(d, 2.0f);
	Vector2f a = calcAcceleration(f, m_mass);
	V = calcSpeed(V, a, diff, maxV, 1.f);

	MoveObject::TimeElapsed(diff);
	_shape.setRadius(_radius);
	_shape.setPosition(Center.x - _shape.getRadius(), Center.y - _shape.getRadius());
	name.setPosition(Center.x - name.getLocalBounds().width / 2, Center.y - name.getLocalBounds().height / 2);
}

Vector2f Hero::getCenter()
{
	return Center;
}