#include "Piece.h"

#include <iostream>

Piece::Piece(Vector2f center, float  radius, int Col, int id) : MoveObject(center, radius, id)
{
	_shape.setRadius(radius);
	_shape.setFillColor(botsArrColor[Col]);
	_shape.setPosition(center.x - radius, center.y - radius);
	Center = center;
	maxV = 0.f;
}

Vector2f Piece::getCenter()
{
	return Center;
}

Vector2f Piece::getSpeed()
{
	return V;
}

float Piece::getMaxV()
{
	return maxV;
}

void Piece::setMaxV(float newMaxV)
{
	maxV = newMaxV;
}

void Piece::draw(RenderWindow& window)
{
	_shape.setRadius(_radius);
	_shape.setPosition(Center.x - getRadius(), Center.y - getRadius());
	window.draw(_shape);
}