#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

struct vert
{
	sf::Vector2f point;
	vert* next = nullptr;
	vert* prev = nullptr;
	vert* nexPoly = nullptr;
	bool intersect = false;
	bool etryExit = false;
	bool visited = false;
	float alpha = 0;

	vert(const sf::Vector2f& p) : point(p) {}
};

vert* createPolygon(const std::vector<sf::Vector2f>& points);

void deletePolygon(vert* poly);

bool segmentIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& q1, const sf::Vector2f& q2, float &t1, float &t2, sf::Vector2f& inter);