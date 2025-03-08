#pragma once
#include "Food.h"
#include "MoveObject.h"
class Feed : public Food
{
public:
	Feed(Vector2f center, float radius, int id);
	void draw(RenderWindow& window);
private:
	CircleShape _shape;
};