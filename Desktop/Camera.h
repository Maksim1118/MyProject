#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera();
	void setTargetZoom(float zoom);
	void updateZoom(int& diff);
	void updateView(const sf::Vector2f center);
	const sf::View getView() const;
	//void setCenter(sf::Vector2f center);
	//sf::Vector2f getCenter() const;
	///*sf::Vector2f getSize() const;*/
	//void updateZoom(float diff, float playerCurRadius, float playerMinRadius);
	//void updateSize(sf::Vector2f screenSize);
	//sf::View getView() const;
private:
	sf::View m_view;
	float m_currZoom;
	float m_targetZoom;
};