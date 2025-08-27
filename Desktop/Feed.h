#pragma once
#include "Food.h"
#include "MoveObject.h"
class Feed : public MoveObject
{
public:
	Feed(Vector2f center, float radius, string id);
	void TimeElapsed(int diff) override;
	void setCenter(Vector2f newCenter) override;
	void shiftPos(float offsetX = 0, float offsetY = 0) override;
	void draw(RenderWindow& window) const;
private:
	CircleShape _shape;
};