#pragma once
#include <SFML/Graphics.hpp>
#include "Objects.h"
using namespace sf;
using namespace std;

class Food: public Objects
{
public:
	Food(Vector2f center, float radius, int id);
	void setColor(int Col);
	void draw(RenderWindow& window) override;
private:
	CircleShape _shape;
};

