#include "TextButton.h"

using namespace sf;
using namespace std;

Color disableColor = Color(100.f, 100.f, 100.f);

TextButton::TextButton(const string& str, const Font& font, const Vector2f& pos, const unsigned int chSize, function<void()> onClick)
	:m_enabled(false)
{
	m_text.setString(str);
	m_text.setFont(font);
	m_text.setFillColor(disableColor);
	m_text.setCharacterSize(chSize);
	setPosition(pos);

	FloatRect localBounds = m_text.getLocalBounds();
	m_text.setOrigin(localBounds.left, localBounds.top);

	this->onClick = std::move(onClick);
}

TextButton::~TextButton()
{
}

TextButton::TextButton(TextButton&& other) noexcept
	:Button(std::move(other))
	,m_text(std::move(other.m_text))
	,m_enabled(exchange(other.m_enabled, false))
{
}

TextButton& TextButton::operator=(TextButton&& other) noexcept
{
	if (this != &other)
	{

		Button::operator=(std::move(other));
		m_text = std::move(other.m_text);
		m_enabled = std::exchange(other.m_enabled,false);
	}
	return *this;
}

void TextButton::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
	if (!m_enabled) return;

	FloatRect bounds = getTransform().transformRect(m_text.getGlobalBounds());
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		if (bounds.contains(mousePos))
		{
			if (onClick)
			{
				onClick();
			}
		}
	}
}

void TextButton::update(const int diff, const sf::Vector2f& mousePos)
{
	if (!m_enabled)
	{
		m_text.setFillColor(disableColor);
		return;
	}
	FloatRect bounds = getTransform().transformRect(m_text.getGlobalBounds());

	if (bounds.contains(mousePos))
	{
		m_text.setFillColor(Color::Red);
	}
	else
	{
		m_text.setFillColor(Color::Black);
	}
}

bool TextButton::isBlocking()
{
	return false;
}

void TextButton::setEnable(const bool enable)
{
	m_enabled = enable;
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_text, states);
}
