#include "Feed.h"

Feed::Feed(Vector2f center, float radius, int id) : Food(center, radius, id)
{
	_shape.setRadius(radius);
	_shape.setPosition(center.x - radius, center.y - radius);
	_shape.setFillColor(Color(255, 100, 255));
}

void Feed::draw(RenderWindow& window)
{
	window.draw(_shape);
}