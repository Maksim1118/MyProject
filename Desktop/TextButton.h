#pragma once

#include "Button.h"

#include <string>

class TextButton : public Button
{
public:
	TextButton(const std::string& str, const sf::Font& font, const sf::Vector2f& pos, const unsigned int chSize, std::function<void()> onClick);
	~TextButton()override;

	TextButton(TextButton&&) noexcept;
	TextButton& operator=(TextButton&&) noexcept;

	
	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
	void update(const int diff, const sf::Vector2f& mousePos) override;
	bool isBlocking() override;
	void setEnable(const bool enable);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Text m_text;
	bool m_enabled;

};