#pragma once

#include <SFML/Graphics.hpp>

class UI_Element: public sf::Drawable, public sf::Transformable
{
public:
	UI_Element() = default;
	virtual ~UI_Element() = default;

	UI_Element(const UI_Element&) = delete;
	UI_Element& operator=(const UI_Element&) = delete;

	UI_Element(UI_Element&&) noexcept = default;
	UI_Element& operator=(UI_Element&&)noexcept = default;

	virtual bool isBlocking() = 0;
	virtual void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) = 0;
	virtual void update(const int diff, const sf::Vector2f& mousePos) = 0;
};