#pragma once
#include "nlohmann\json.hpp"
#include "Objects.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class MoveObject : public Objects
{
public:
	MoveObject(Vector2f center, float radius, int id);
	void TimeElapsed(int diff) override;
	void setSpeed(Vector2f newSpeed);
	void setCenter(Vector2f newCenter);
	void setDifference(Vector2f diff);
	Vector2f getDiff();
protected:
	float GetLen(const Vector2f& p1, const Vector2f& p2);
	float GetLen(const Vector2f& vector);
	Vector2f getIdentityVector(const Vector2f& vector);
	Vector2f difference;
	Vector2f V;
	Vector2f Mouse;
};