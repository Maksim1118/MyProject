#pragma once
#include <SFML/Graphics.hpp>

#include <functional>

namespace SFML_SIMPLIFIED
{
	void initSprite(sf::Sprite& outSprite, const sf::Texture& texture, const sf::Vector2f& origin, const sf::Vector2f& pos, const sf::Vector2f& size);
	sf::Color lerpColor(const sf::Color& a, const sf::Color& b, const float t);
	float approachWithClamp(float cur, float target, int diff, float speed, float minVal, float maxVal);
}