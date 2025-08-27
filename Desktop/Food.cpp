#include "Food.h"

Color arrcolor[] =
{ Color(250, 255, 0),
	Color(255, 0, 0),
	Color(0, 199, 255),
	Color(100, 250, 50),
	Color(165, 72, 38),
	Color(168, 17, 255) };


Food::Food(Vector2f center, float radius, string id): Objects(Vector2f(0,0), 0, id)
{
	_shape.setRadius(radius);
	_shape.setPosition(center.x - radius, center.y - radius);	
}

void Food::setColor(Color color)
{
	_shape.setFillColor(color);
}


void Food::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}

void Food::shiftPos(float offsetX, float offsetY)
{
	_shape.setPosition(Center.x - _radius + offsetX, Center.y - _radius + offsetY);
}


void Food::draw(RenderWindow& window) const
{	
	window.draw(_shape);
}
