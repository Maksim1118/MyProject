#pragma once
#include <SFML/Graphics.hpp>

#include "Objects.h"
#include "ResourñeManager.h"

using namespace sf;
using namespace std;

constexpr int ThornSpriteSize = 10;
constexpr float _ThornSpriteMass = 2500.f;
const float _ThornSpriteR = sqrt(_ThornSpriteMass);

class Thorn : public Objects
{
public:
	Thorn(Vector2f center,  float radius, int id);
	void TimeElapsed(int diff);
	void draw(RenderWindow& window);
private:
	Sprite _ThornSprite;	
};

