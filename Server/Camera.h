#pragma once

#include <SFML/Graphics.hpp>

namespace Server
{
	class Camera
	{
	public:
		Camera(sf::Vector2f vSize);

		/*void updateSize(sf::Vector2f screenSize);*/
		sf::View getView() const;
		sf::Vector2f m_viewSize;
		
	};
}
