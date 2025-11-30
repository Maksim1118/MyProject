#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

constexpr int fieldWidth = 2500;
constexpr int fieldHeight = 2500;

using namespace sf;

namespace Server
{
	float GetLen(const Vector2f& p1, const Vector2f& p2);
	float GetLen(const Vector2f& vector);
	float GetCyclicDist(const Vector2f& p1, const Vector2f& p2, float sizeW, float sizeH);
	std::vector<sf::FloatRect> normalizeRect(const sf::FloatRect& r, float sizeW, float sizeH);


	bool containsRect(const sf::FloatRect& outer, const sf::FloatRect& inner);
	bool cyclicIntersectsRect(const sf::FloatRect& r1, const sf::FloatRect& r2, float sizeW, float sizeH);
	float wrapCoordinate(float coord, float size);

	sf::Vector2f calcAttractionForce(const sf::Vector2f& dir, float koef);
	sf::Vector2f calcAcceleration(const sf::Vector2f& F, const float mass);
	sf::Vector2f calcSpeed(const sf::Vector2f& v0, const sf::Vector2f& a, int diff, float maxV, float koef);

	sf::Vector2f findCyclicCenter(const std::vector<sf::Vector2f>& centers);
	float distanceToMapBorder(const sf::Vector2f& center, const sf::Vector2f& dir);
	sf::Vector2f identityVector(sf::Vector2f vec);
	sf::Vector2f GetCyclicDiff(const Vector2f& from, const Vector2f& to, float sizeW, float sizeH);
	Vector2f generateDir();
	sf::Vector2f normalizeCoord(const sf::Vector2f& vec);
	Vector2f getIdentityVector(const Vector2f& vector);
	bool circleIntersectCircle(const sf::Vector2f& c1, const float r1, const sf::Vector2f& c2, const float r2);
}