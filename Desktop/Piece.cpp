#include "Piece.h"

#include <iostream>

Piece::Piece(Vector2f center, float  radius, int Col, string id, const string& parentId) : MoveObject(center, radius, id), m_parentId(parentId)
{
	_shape.setRadius(radius);
	_shape.setFillColor(botsArrColor[Col]);
	_shape.setPosition(center.x - radius, center.y - radius);
	Center = center;
	maxV = 0.f;
	m_isExcluded = false;
}

void Piece::TimeElapsed(int diff)
{

	/*float len = GetLen(Mouse);
	float radius = getRadius();

	float localMaxV = maxV;

	if (len < radius)
	{
		localMaxV *= (len / radius);
	}

	Vector2f d = getIdentityVector(Mouse);
	Vector2f f = calcAttractionForce(d, 0.3f);
	Vector2f a = calcAcceleration(f, m_mass);
	V = calcSpeed(V, a, diff, localMaxV, 1.f);*/

	MoveObject::TimeElapsed(diff);
	_shape.setRadius(_radius);
	_shape.setPosition(Center.x - _radius, Center.y - _radius);
	/*cout << "CenterPiece: " << Center.x << " " << Center.y << endl;*/
}

void Piece::draw(RenderWindow& window) const
{
	window.draw(_shape);
}

void Piece::shiftPos(float offsetX, float offsetY)
{
	_shape.setPosition(Center.x - _radius + offsetX, Center.y - _radius + offsetY);
}

Vector2f Piece::getCenter()
{
	return Center;
}

Vector2f Piece::getSpeed()
{
	return V;
}

void Piece::setExclude(bool exclude)
{
	m_isExcluded = exclude;
}

bool Piece::isExclude() const
{
	return m_isExcluded;
}

void Piece::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}
