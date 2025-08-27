#include "Hero.h"
#include <iostream>
//#include "BaseOld.h"

#include "ResourñeManager.h"

Hero::Hero(Vector2f center, float radius, int Col, string text, string id): Bot(center, radius, Col, text, id)
{
	m_IsGameOver = false;
}

void Hero::TimeElapsed(int diff)
{	
	Vector2f d = getIdentityVector(Mouse - Center);
	Vector2f a = d * 0.00014f;

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
	}
	MoveObject::TimeElapsed(diff);
	_shape.setRadius(_radius);
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
//#include "ResourñeManager.h"
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