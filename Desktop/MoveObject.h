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
	Vector2f calcAttractionForce(const sf::Vector2f& dir, float koef);
	Vector2f calcAcceleration(const Vector2f& F, const float mass);
	Vector2f calcSpeed(const Vector2f& v0, const Vector2f& a, int diff, float maxV, float koef);
	sf::Vector2f GetCyclicDiff(const Vector2f& from, const Vector2f& to, float sizeW, float sizeH);
	float toroidalDiff(float diff, float size);
	static Vector2f getIdentityVector(const Vector2f& vector);
	void setDirection(const sf::Vector2f& dir);
	void setMaxV(float _maxV);
protected:
	Vector2f getDirection() const;
protected:
	Vector2f difference;
	Vector2f V;
	Vector2f Mouse;
	float maxV;
};