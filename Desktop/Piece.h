#pragma once
#include "Objects.h"
#include "MoveObject.h"

extern  Color botsArrColor[];

class Piece : public MoveObject
{
public:
	Piece(Vector2f center, float  radius, int Col, int id);
	void draw(RenderWindow& window)override;
	Vector2f getCenter();
	Vector2f getSpeed();
	float getMaxV();
	void setMaxV(float newMaxV);
private:
	CircleShape _shape;
	float maxV;

};