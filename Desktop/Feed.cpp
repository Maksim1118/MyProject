#include "Feed.h"

Feed::Feed(Vector2f center, float radius, string id) : MoveObject(center, radius, id)
{
	_shape.setRadius(radius);
	_shape.setPosition(center.x - radius, center.y - radius);
	_shape.setFillColor(Color(255, 100, 255));
}

void Feed::TimeElapsed(int diff)
{
	float lenV = GetLen(V);
	float newlenV = lenV - 0.0005f * (float)diff;
	if (newlenV <= 0)
	{
		V = { 0,0 };
	}
	else
	{
		V *= newlenV / lenV;

	}
	MoveObject::TimeElapsed(diff);
}


void Feed::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}

void Feed::shiftPos(float offsetX, float offsetY)
{
	_shape.setPosition(Center.x - _shape.getRadius(), Center.y - _shape.getRadius());
}


void Feed::draw(RenderWindow& window) const
{
	window.draw(_shape);
}
