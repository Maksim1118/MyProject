#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using ColorsFood = std::vector<sf::Color>;

inline const ColorsFood getFoodColors()
{
	return
	{
        sf::Color(0, 255, 0),   // green
        sf::Color(255, 0, 0),   // red
        sf::Color(255, 255, 0), // yellow
        sf::Color(166, 0, 247), // purple
        sf::Color(21, 96, 189), // denimBlue
        sf::Color(0, 105, 62)   // darkGreen
	};
}