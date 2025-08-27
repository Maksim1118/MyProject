#include "HoverButton.h"

#include "ResourñeManager.h"

#include <iostream>
#include <cassert>

using namespace sf;
using namespace std;

HoverButton::HoverButton(const Vector2f& pos, const Vector2f& size)
	:
	m_normalTex(rec.textures[ResourceManager::Textures::BUTTONCLOSE])
	,m_hoverTex(rec.textures[ResourceManager::Textures::BUTTONCLOSEPRESSED])
{
	assert(m_normalTex && "normalTex must not be nullptr");
	assert(m_hoverTex && "hoverTex texture must not be nullptr");

	m_sprite.setTexture(*m_normalTex);
	setSize(size.x, size.y);
	setPosition(pos);
}
HoverButton::~HoverButton()
{
}
HoverButton::HoverButton(HoverButton&& other) noexcept
	: Button(std::move(other))
	, m_normalTex(exchange(other.m_normalTex, nullptr))
	, m_hoverTex(exchange(other.m_hoverTex, nullptr))
	, m_sprite(std::move(other.m_sprite))
	, m_isHover(other.m_isHover)
{

}
HoverButton& HoverButton::operator=(HoverButton&& other) noexcept
{
	if (this != &other)
	{
		Button::operator=(std::move(other));

		m_normalTex = exchange(other.m_normalTex, nullptr);
		m_hoverTex = exchange(other.m_hoverTex, nullptr);
		m_sprite = std::move(other.m_sprite);
		m_isHover = other.m_isHover;

	}
	return *this;
}
//HoverButton::HoverButton(HoverButton&& other) noexcept
//	: Drawable(std::move(other)), Transformable(std::move(other))
//	,m_sprite(std::move(other.m_sprite))
//	,m_isHover(other.m_isHover)
//	,m_normalTex(other.m_normalTex)
//	,m_hoverTex(other.m_hoverTex)
//{
//	other.m_normalTex = nullptr;
//	other.m_hoverTex = nullptr;
//}
//HoverButton& HoverButton::operator=(HoverButton&& other) noexcept
//{
//	if (this != &other)
//	{
//		Drawable::operator=(std::move(other));
//		Transformable::operator=(std::move(other));
//		m_sprite = std::move(other.m_sprite);
//		m_isHover = other.m_isHover;
//		m_normalTex = other.m_normalTex;
//		m_hoverTex = other.m_hoverTex;
//
//		other.m_normalTex = nullptr;
//		other.m_hoverTex = nullptr;
//	}
//	return *this;
//}

void HoverButton::handleEvent(const Event& event, const Vector2f& mousePos)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		FloatRect bounds = getTransform().transformRect(m_sprite.getGlobalBounds());

		if (bounds.contains(mousePos))
		{
			if (onClick)
			{
				onClick();
			}
		}
	}
}

void HoverButton::update(const int diff, const sf::Vector2f& mousePos)
{
	FloatRect bounds = getTransform().transformRect(m_sprite.getGlobalBounds());

	if (bounds.contains(mousePos))
	{
		if (!m_isHover)
		{
			m_isHover = true;
			m_sprite.setTexture(*m_hoverTex);
		}
	}
	else
	{
		if (m_isHover)
		{
			m_isHover = false;
			m_sprite.setTexture(*m_normalTex);
		}
	}
}

bool HoverButton::isBlocking()
{
	return false;
}

void HoverButton::setSize(float width, float height)
{
	Vector2u texSize = m_sprite.getTexture()->getSize();
	float scaleX = width / texSize.x;
	float scaleY = height / texSize.y;
	m_sprite.setScale(scaleX, scaleY);
}

sf::Vector2f HoverButton::getSize() const
{
	FloatRect bounds = m_sprite.getGlobalBounds();
	return { bounds.width, bounds.height };
}

void HoverButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}
