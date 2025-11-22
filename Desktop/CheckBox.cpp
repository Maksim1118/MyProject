#include "CheckBox.h"

#include "ResourñeManager.h"
#include "SimplifiedSfmlFunc.h"

#include <cassert>

using namespace sf;
using namespace std;

constexpr float scaleSpeed = 1e-2f;
constexpr float maxScale = 1.4f;
constexpr float minScale = 1.f;

CheckBox::CheckBox(const sf::Vector2f& pos, const float sideSize,const bool checked, function<void(bool)> onChanged)
	:normBoxTex(rec.textures[ResourceManager::Textures::BOX])
	,blurBoxTex(rec.textures[ResourceManager::Textures::BLUR_BOX])
	,checkMarkTex(rec.textures[ResourceManager::Textures::CHECKMARK])
	,m_checked(checked)
	,onChanged(std::move(onChanged))
	,m_hovered(false)
	,m_curScale(1.f)
	,m_baseScale_Box(1.f)
	,m_baseScale_CheckMark(1.f)
{

	assert(normBoxTex && "normBoxTex must not be nullptr");
	assert(blurBoxTex && "blurBoxTex texture must not be nullptr");
	assert(checkMarkTex && "checkMarkTex texture must not be nullptr");


	m_Box.setTexture(*normBoxTex);
	m_checkMark.setTexture(*checkMarkTex);

	FloatRect localBounds = m_checkMark.getLocalBounds();
	FloatRect boxLocalBounds = m_Box.getLocalBounds();

	m_Box.setOrigin(boxLocalBounds.width / 2.f, boxLocalBounds.height / 2.f);
	m_checkMark.setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
	m_checkMark.setPosition(m_Box.getPosition());
	setSize(sideSize);
	setPosition(pos);

}

CheckBox::~CheckBox()
{
}

CheckBox::CheckBox(CheckBox&& other) noexcept
	:UI_Element(std::move(other)),

	normBoxTex(other.normBoxTex),
	blurBoxTex(other.blurBoxTex),
	checkMarkTex(other.checkMarkTex),
	m_checked(other.m_checked),
	m_hovered(other.m_hovered),
	m_curScale(other.m_curScale),
	m_baseScale_Box(other.m_baseScale_Box),
	m_baseScale_CheckMark(other.m_baseScale_CheckMark),
	onChanged(std::move(other.onChanged)),

	m_Box(std::move(other.m_Box)),
	m_checkMark(std::move(other.m_checkMark))
{
	other.normBoxTex = nullptr;
	other.blurBoxTex = nullptr;
	other.checkMarkTex = nullptr;

	other.m_checked = false;
	other.m_hovered = false;
	other.m_curScale = 0.f;
	other.m_baseScale_Box = 0.f;
	other.m_baseScale_CheckMark = 0.f;

	other.onChanged = nullptr;
}

CheckBox& CheckBox::operator=(CheckBox&&other) noexcept
{
	if (this != &other)
	{
		UI_Element::operator=(std::move(other));

		normBoxTex = other.normBoxTex;
		blurBoxTex = other.blurBoxTex;
		checkMarkTex = other.checkMarkTex;
		m_checked = other.m_checked;
		m_hovered = other.m_hovered;
		m_curScale = other.m_curScale;
		m_baseScale_Box = other.m_baseScale_Box;
		m_baseScale_CheckMark = other.m_baseScale_CheckMark;
		onChanged = std::move(other.onChanged);
		m_Box = std::move(other.m_Box);
		m_checkMark = std::move(other.m_checkMark);

		other.normBoxTex = nullptr;
		other.blurBoxTex = nullptr;
		other.checkMarkTex = nullptr;
		other.m_checked = false;
		other.m_hovered = false;
		other.m_curScale = 0.f;
		other.m_baseScale_Box = 0.f;
		other.m_baseScale_CheckMark = 0.f;
		other.onChanged = nullptr;
	}
	return *this;
}



void CheckBox::handleEvent(const Event& event, const Vector2f& mousePos)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		FloatRect bounds = getTransform().transformRect(m_Box.getGlobalBounds());
		if (bounds.contains(mousePos))
		{
			m_checked = !m_checked;
			if (onChanged)
			{
				onChanged(m_checked);
			}
		}
	}
}


void CheckBox::setSize(const float sideSize)
{
	Vector2u boxSize = m_Box.getTexture()->getSize();
	m_baseScale_Box = min(sideSize / boxSize.x, sideSize / boxSize.y);
	m_Box.setScale(m_baseScale_Box, m_baseScale_Box);

	Vector2u checkMarkSize = m_checkMark.getTexture()->getSize();
	m_baseScale_CheckMark = min(sideSize / checkMarkSize.x, sideSize / checkMarkSize.y);
	m_checkMark.setScale(m_baseScale_CheckMark, m_baseScale_CheckMark);
}

void CheckBox::update(const int diff, const sf::Vector2f& mousePos)
{
	FloatRect bounds = getTransform().transformRect(m_Box.getGlobalBounds());
	m_hovered = bounds.contains(mousePos);

	float targetScale = m_hovered ? maxScale : minScale;
	m_curScale = SFML_SIMPLIFIED::approachWithClamp(m_curScale, targetScale, diff, scaleSpeed, minScale, maxScale);

	m_Box.setScale(m_baseScale_Box * m_curScale, m_baseScale_Box * m_curScale);	
	m_checkMark.setScale(m_baseScale_CheckMark * m_curScale, m_baseScale_CheckMark * m_curScale);;

	m_Box.setTexture(m_hovered ? *blurBoxTex : *normBoxTex);
}

bool CheckBox::isBlocking()
{
	return false;
}


void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_Box, states);
	if (m_checked)
	{
		target.draw(m_checkMark, states);
	}
}

void CheckBox::positionAdjusment()
{
	m_checkMark.setPosition(m_Box.getPosition());
}

