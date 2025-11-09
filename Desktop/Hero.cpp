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
	/*float kF = 100.f;
	if (len < radius) {
		kF = len / radius;
	}*/

	Vector2f d = getIdentityVector(Mouse);
	Vector2f f = calcAttractionForce(d, 2.0f);
	Vector2f a = calcAcceleration(f, m_mass);
	V = calcSpeed(V, a, diff, maxV, 1.f);

	/*Vector2f a = d * 0.00014f;

	V += a * (float)diff;
	float lenV = GetLen(V);
	float len = GetLen(Mouse - Center);
	float maxV = 0.04f;
	if (len < getRadius())
	{
		maxV *= (len / getRadius());
	}
	if (lenV > maxV)
	{
		V = V / lenV * maxV;
	}*/

	MoveObject::TimeElapsed(diff);
	_shape.setRadius(_radius);
	_shape.setPosition(Center.x - _shape.getRadius(), Center.y - _shape.getRadius());
	name.setPosition(Center.x - name.getLocalBounds().width / 2, Center.y - name.getLocalBounds().height / 2);
}

Vector2f Hero::getCenter()
{
	return Center;
}
//
//#include "Hero.h"
//#include <iostream>
////#include "BaseOld.h"
//
//#include "ResourсeManager.h"
//
//Hero::Hero(Vector2f center, float radius, int Col, string text, string id) : Bot(center, radius, Col, text, id)
//{
//	m_IsGameOver = false;
//}
//
//void Hero::TimeElapsed(int diff)
//{
//	if (splitted == true)
//	{
//		for (auto& piece : pieces)
//		{
//			Vector2f d = getIdentityVector(Mouse - Center);
//			Vector2f a = d * 0.00014f;
//
//			piece->setSpeed(piece->getSpeed() + a * (float)diff);
//			float lenV = GetLen(piece->getSpeed(), Vector2f(0, 0));
//			float len = GetLen(Mouse - Center);
//			if (lenV > piece->getMaxV())
//			{
//				piece->setSpeed(piece->getSpeed() / lenV * piece->getMaxV());
//			}
//		}
//		pieceToSides();
//	}
//	else
//	{
//		Vector2f d = getIdentityVector(Mouse - Center);
//		Vector2f a = d * 0.00014f;
//
//		V += a * (float)diff;
//		float lenV = GetLen(V, Vector2f(0, 0));
//		float len = GetLen(Mouse - Center);
//		float maxV = 0.04f;
//		if (len < getRadius())
//		{
//			maxV *= (len / getRadius());
//		}
//		if (lenV > maxV)
//		{
//			V = V / lenV * maxV;
//		}
//	}
//	MoveObject::TimeElapsed(diff);
//
//}
//
//
//void Hero::draw(RenderWindow& window)
//{
//	Bot::draw(window);
//}
//
//Vector2f Hero::getCenter()
//{
//	return Center;
//}