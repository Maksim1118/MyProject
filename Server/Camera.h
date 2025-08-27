#pragma once

#include <SFML/Graphics.hpp>

namespace Server
{
	class Camera
	{
	public:
		Camera(sf::Vector2f fieldSize);
		void setCenter(sf::Vector2f center);
		sf::Vector2f getCenter() const;


		sf::Vector2i getLogicSize() const;
		float getZoom(float playerCurMass, float playerMinMass);


		void updateSize(sf::Vector2f screenSize);
		sf::View getView() const;
	private:
		void limitZoomByFieldSize();
		sf::View m_view;
		float m_currZoom;
		sf::Vector2f m_FieldSize;
		
	};
}
