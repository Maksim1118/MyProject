#include "Feed.h"

Feed::Feed(Vector2f center, float radius, int id) : MoveObject(center, radius, id)
{
	_shape.setRadius(radius);
	_shape.setPosition(center.x - radius, center.y - radius);
	_shape.setFillColor(Color(255, 100, 255));
}

void Feed::TimeElapsed(int diff)
{
	/*if (GetLen(V) != 0.f)*/
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
	/*if (GetLen(parentCenter, getCenter()) > parentRadius)
	{
		float lenV = GetLen(V);
		float newlenV = lenV - 0.001f * diff;
		if (newlenV <= 0)
		{
			V = { 0,0 };
		}
		else
		{
			V *= newlenV / lenV;

		}
	}*/
}

void Feed::draw(RenderWindow& window)
{
	_shape.setPosition(Center.x - _shape.getRadius(), Center.y - _shape.getRadius());
	window.draw(_shape);
}