#include "Thorn.h"

#include <iostream>

Thorn::Thorn(Vector2f center, float radius, string id): Objects(Vector2f(0, 0),radius,id)
{
	_ThornSprite.setTexture(*rec.textures[ResourceManager::Textures::THORN]);
	_ThornSprite.setPosition(center.x - radius, center.y - radius);
}


void Thorn::draw(RenderWindow& window) const
{
	window.draw(_ThornSprite);
}

void Thorn::shiftPos(float offsetX, float offsetY)
{
	_ThornSprite.setPosition(Center.x - _radius + offsetX, Center.y - _radius + offsetY);
}


void Thorn::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}

