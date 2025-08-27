#pragma once

#include "UI_Element.h"

#include <functional>

class Button : public UI_Element
{
public:
	Button() = default;
	virtual ~Button() = default;

	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;

	Button(Button&&) noexcept = default;
	Button& operator=(Button&&) noexcept = default;

	virtual void update(const int diff, const sf::Vector2f& mousePos)override = 0;
	virtual void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override = 0;
	std::function<void()> onClick;
};