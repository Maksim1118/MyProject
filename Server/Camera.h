#pragma once

#include <SFML/Graphics.hpp>

namespace Server
{
	class Camera
	{
	public:
		Camera(sf::Vector2f vSize);
		sf::View getView() const;
		sf::Vector2f m_viewSize;
		
	};
}
