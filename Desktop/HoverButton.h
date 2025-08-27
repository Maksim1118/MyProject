#pragma once

#include "Button.h"

class HoverButton : public Button
{
public:
	HoverButton(const sf::Vector2f& pos, const sf::Vector2f& size);
	~HoverButton()override;

	HoverButton(HoverButton&&)noexcept;
	HoverButton& operator=(HoverButton&&)noexcept;

	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
	void update(const int diff, const sf::Vector2f& mousePos) override;
	bool isBlocking() override;
	void setSize(const float width, const float height);
	sf::Vector2f getSize() const;


private:
	const sf::Texture* m_normalTex;
	const sf::Texture* m_hoverTex;
	sf::Sprite m_sprite;
	bool m_isHover;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};