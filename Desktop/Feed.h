#pragma once
#include "Food.h"
#include "MoveObject.h"
class Feed : public MoveObject
{
public:
	Feed(Vector2f center, float radius, int id);
	void TimeElapsed(int diff) override;
	void draw(RenderWindow& window);
private:
	CircleShape _shape;
};