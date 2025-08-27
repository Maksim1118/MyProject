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
	Hero(Vector2f center, float radius, int Col, string text, string id);
	Vector2f getCenter();
	void TimeElapsed(int diff) override;
	void setCurrentMass(float mass);
	bool m_IsGameOver;
};
