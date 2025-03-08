#pragma once
#include "Bot.h"
#include <list>
#include "Piece.h"
#include "Food.h"

using namespace sf;
using namespace std;

class Hero:public Bot
{
public:
	Hero(Vector2f center, float radius, int Col, string text, int id);
	Vector2f getCenter();
	void TimeElapsed(int diff) override;
};
