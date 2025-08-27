#pragma once
#include <SFML/Graphics.hpp>

constexpr int fieldWidth = 2500;
constexpr int fieldHeight = 2500;

using namespace sf;

float GetLen(const Vector2f& p1, const Vector2f& p2);
float GetLen(const Vector2f& vector);
float GetDist(const Vector2f& p1, const Vector2f& p2);

sf::Vector2f findCyclicCenter(const std::vector<sf::Vector2f>& centers);
float distanceToMapBorder(const sf::Vector2f& center, const sf::Vector2f& dir);
sf::Vector2f identityVector(sf::Vector2f vec);
sf::Vector2f GetDiff(const Vector2f& p1, const Vector2f& p2);
sf::Vector2f normalizeCoord(const sf::Vector2f& vec);
Vector2f getIdentityVector(const Vector2f& vector);
bool circleIntersectCircle(const sf::Vector2f& c1, const float r1, const sf::Vector2f& c2, const float r2);