#pragma once
#include <SFML/Graphics.hpp>
#include "Objects.h"
using namespace sf;
using namespace std;

class Food: public Objects
{
public:
	Food(Vector2f center, float radius, string id);
	void setColor(sf::Color color);
	void setCenter(Vector2f newCenter) override;
	void shiftPos(float offsetX = 0, float offsetY = 0) override;
	void draw(RenderWindow& window) const override;
private:
	CircleShape _shape;
};

