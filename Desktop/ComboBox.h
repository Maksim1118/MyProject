#pragma once

#include "UI_Element.h"

#include <memory>

#include <vector>
#include <functional>
#include <string>

class ComboBox: public UI_Element
{
public:
	ComboBox(const sf::Font& font, const std::vector<std::string>& strItems, const unsigned int selectedInd, const sf::Vector2f& size, const sf::Vector2f& pos, std::function<void(std::string&)> onChanged);
	
	ComboBox(ComboBox&&) noexcept;
	ComboBox& operator=(ComboBox&&) noexcept;
	
	~ComboBox()override;

	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
	void update(const int diff, const sf::Vector2f& mousePos) override;
	bool isBlocking() override;

	void setSize(const float width, const float height);
private:
	struct Layout
	{
		sf::RectangleShape backGround;
		std::vector<sf::Text> textItems;
	};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void scroll(const float delta);
	void toggleDropDown();
	void createLayout();
	void deleteLayout();

	void updateSelectedText();
	
	int calcHoveredInd(const sf::Vector2f& mousePos);
private:
	const sf::Texture* frameTex;
	const sf::Texture* arrowUpTex;
	const sf::Texture* arrowDownTex;

	std::vector<std::string> m_items;

	sf::Font m_font;

	sf::Vector2f m_size;

	sf::Text m_selectedText;

	std::function<void(std::string&)> onChanged;

	bool m_expand;

	int m_hoveredInd;

	bool m_highlightArrow;

	unsigned int m_selectedInd;
	unsigned int m_charSize;

	float m_scrollOff;
	float m_maxScrollOffset;

	float m_itemHeightInLayout;

	Layout m_layout;

	mutable std::unique_ptr<sf::RenderTexture> m_renderTexture;

	sf::Sprite m_frame;
	sf::Sprite m_arrow;
};