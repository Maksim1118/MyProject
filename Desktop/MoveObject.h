#pragma once
#include "nlohmann\json.hpp"
#include "Objects.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class MoveObject : public Objects
{
public:
	MoveObject(Vector2f center, float radius, string id);
	void TimeElapsed(int diff) override;
	void setSpeed(Vector2f newSpeed);
	void setDifference(Vector2f diff);
	Vector2f getDiff();
	static float GetLen(const Vector2f& p1, const Vector2f& p2, const Vector2f& mapSize);
	static float GetLen(const Vector2f& vector);
	static sf::Vector2f identityVector(const sf::Vector2f& vec);
	static Vector2f getIdentityVector(const Vector2f& vector);
protected:
	Vector2f difference;
	Vector2f V;
	Vector2f Mouse;
};