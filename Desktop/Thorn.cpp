#include "Thorn.h"

#include <iostream>

Thorn::Thorn(Vector2f center, float radius, int id): Objects(Vector2f(0, 0),radius,id)
{
	_ThornSprite.setTexture(*rec.textures[ResourceManager::Textures::THORN]);
	_ThornSprite.setPosition(center.x - radius, center.y - radius);
}

void Thorn::TimeElapsed(int diff) 
{

}

void Thorn::draw(RenderWindow& window)
{
	window.draw(_ThornSprite);		
}
