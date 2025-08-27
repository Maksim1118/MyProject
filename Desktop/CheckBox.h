#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

#include "UI_Element.h"

class CheckBox : public UI_Element
{
public:
	CheckBox(const sf::Vector2f& pos, const float sideSize, const bool checked, std::function<void(bool)> onChanged);
	~CheckBox() override;

	CheckBox(CheckBox&&) noexcept;
	CheckBox& operator=(CheckBox&&) noexcept;

	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
	void update(const int diff, const sf::Vector2f& mousePos) override;
	bool isBlocking() override;

	void setSize(const float sideSize);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void positionAdjusment();
private:
	const sf::Texture* normBoxTex;
	const sf::Texture* blurBoxTex;
	const sf::Texture* checkMarkTex;
	bool m_checked;
	bool m_hovered;
	float m_curScale;
	float m_baseScale_Box;
	float m_baseScale_CheckMark;
	std::function<void(bool)> onChanged;

	sf::Sprite m_Box;
	sf::Sprite m_checkMark;
};


