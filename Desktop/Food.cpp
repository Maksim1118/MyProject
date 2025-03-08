#include "Food.h"

Color arrcolor[] =
{ Color(250, 255, 0),
	Color(255, 0, 0),
	Color(0, 199, 255),
	Color(100, 250, 50),
	Color(165, 72, 38),
	Color(168, 17, 255) };


Food::Food(Vector2f center, float radius, int id): Objects(Vector2f(0,0), 0, id)
{
	_shape.setRadius(radius);
	_shape.setPosition(center.x - radius, center.y - radius);	
}

void Food::setColor(int Col)
{
	_shape.setFillColor(arrcolor[Col]);
}

void Food::draw(RenderWindow& window)
{	
	window.draw(_shape);
}
