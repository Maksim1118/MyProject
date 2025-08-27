#pragma once
#include "Button.h"

#include <string>
#include <functional>

class KeyBindController;

class CharButton: public Button
{
public:
	CharButton(int keyCode, const sf::Font& font, unsigned int chSize, const sf::Vector2f& pos, std::function<void(int)> onChanged);
	~CharButton()override;

	CharButton(CharButton&&) noexcept;
	CharButton& operator =(CharButton&&) noexcept;
	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
	void update(const int diff, const sf::Vector2f& mousePos) override;
	bool isBlocking()override;
	void setChSize(const unsigned int chSize);
private:
	void updatePositionText();
	void setKeyCode(const int keyCode = -1);
	void resetText();
	std::string keyToString(const int keyCode) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const sf::Texture* frameNormTex;
	const sf::Texture* frameBlurTex;

	sf::Font m_font;

	bool m_hovered;

	float m_curScale;

	bool isBindingProcess;

	sf::Text m_text;

	std::function<void(int)> onChanged;

	int m_keyCode;
	unsigned int m_charSize;
	
	sf::Sprite m_frame;


	friend class KeyBindController;
};
