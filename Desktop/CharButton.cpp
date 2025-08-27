#include "CharButton.h"

#include "ResourñeManager.h"
#include "SimplifiedSfmlFunc.h"

#include <iostream>
#include <cctype>
#include <cassert>

using namespace sf;
using namespace std;

constexpr float horizontalPadding = 6.f;

constexpr float speedScale = 0.2f;
constexpr float maxScale = 1.2f;
constexpr float minScale = 1.f;

constexpr unsigned int MAX_COUNT_SYMBOLS = 5;

CharButton::CharButton(int keyCode, const Font& font, unsigned int chSize, const sf::Vector2f& pos, std::function<void(int)> onChanged)
	:frameNormTex(rec.textures[ResourceManager::Textures::BUTTON_FRAME_NORM])
	, frameBlurTex(rec.textures[ResourceManager::Textures::BUTTON_FRAME_BLUR])
	, m_keyCode(keyCode)
	, m_font(font)
	, m_hovered(false)
	, m_curScale(minScale)
	, m_charSize(chSize)
	, onChanged(std::move(onChanged))
	, isBindingProcess(false)

{
	assert(frameNormTex && "frameNormTex must not be nullptr");
	assert(frameBlurTex && "frameBlurTex texture must not be nullptr");

	m_frame.setTexture(*frameNormTex);

	FloatRect localBoundsFrame = m_frame.getLocalBounds();
	m_frame.setOrigin(localBoundsFrame.width / 2.f, localBoundsFrame.height / 2.f);
	
	m_text.setFont(m_font);
	m_text.setString(keyToString(m_keyCode));
	m_text.setFillColor(Color::White);

	setChSize(chSize);

	setPosition(pos);
}

CharButton::~CharButton()
{
}

CharButton::CharButton(CharButton&& other) noexcept
	:Button(std::move(other))
	,frameNormTex(exchange(other.frameNormTex, nullptr))
	,frameBlurTex(exchange(other.frameBlurTex, nullptr))
	,m_font(std::move(other.m_font))
	,m_hovered(exchange(other.m_hovered, false))
	,m_curScale(exchange(other.m_curScale, 0.f))
	,m_text(std::move(other.m_text))
	,onChanged(std::move(other.onChanged))
	,m_keyCode(other.m_keyCode)
	,m_charSize(exchange(other.m_charSize, 0))
	,m_frame(std::move(other.m_frame))
	,isBindingProcess(exchange(other.isBindingProcess, false))

{
	updatePositionText();
}

CharButton& CharButton::operator=(CharButton&& other) noexcept
{
	if (this != &other)
	{

		Button::operator=(std::move(other));

		frameNormTex = std::exchange(other.frameNormTex, nullptr);
		frameBlurTex = std::exchange(other.frameBlurTex, nullptr);
		m_font = std::move(other.m_font);
		m_hovered = std::exchange(other.m_hovered, false);
		m_curScale = std::exchange(other.m_curScale, 0.f);
		m_text = std::move(other.m_text);
		onChanged = std::move(other.onChanged);
		m_keyCode = other.m_keyCode; 
		m_charSize = std::exchange(other.m_charSize, 0);
		m_frame = std::move(other.m_frame);
		isBindingProcess = exchange(other.isBindingProcess, false);

		updatePositionText();
	}
	return *this;
}


void CharButton::handleEvent(const Event& event, const Vector2f& mousePos)
{
	FloatRect bounds = getTransform().transformRect(m_frame.getGlobalBounds());
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

void CharButton::update(const int diff, const Vector2f& mousePos)
{
	FloatRect frameBounds = getTransform().transformRect(m_frame.getGlobalBounds());
	m_hovered = frameBounds.contains(mousePos);

	if (isBindingProcess)
	{
		m_curScale = maxScale;
		m_frame.setTexture(*frameBlurTex);
	}
	else
	{
		float targetScale = m_hovered ? maxScale : minScale;
		m_curScale = SFML_SIMPLIFIED::approachWithClamp(m_curScale, targetScale, diff, speedScale, minScale, maxScale);

		m_frame.setTexture(m_hovered ? *frameBlurTex : *frameNormTex);
	}

	setScale(m_curScale, m_curScale);
}

bool CharButton::isBlocking()
{
	return isBindingProcess;
}

void CharButton::setChSize(const unsigned int chSize)
{
	if (chSize < 5)
	{
		m_charSize = 5;
	}
	else
	{
		m_charSize = chSize;
	}
	
	sf::Text maxSymbol;
	maxSymbol.setFont(m_font);
	maxSymbol.setCharacterSize(m_charSize);
	maxSymbol.setString("W");
	float maxLength = maxSymbol.getLocalBounds().width * MAX_COUNT_SYMBOLS + horizontalPadding;

	m_text.setCharacterSize(m_charSize);

	m_frame.setScale(maxLength / m_frame.getTexture()->getSize().x, static_cast<float>(m_charSize) * 1.4f / m_frame.getTexture()->getSize().y);

	FloatRect localBoundsFrame = m_frame.getLocalBounds();
	m_frame.setOrigin(localBoundsFrame.width / 2.f, localBoundsFrame.height / 2.f);

	updatePositionText();
}

void CharButton::updatePositionText()
{
	FloatRect localBoundsText = m_text.getLocalBounds();
	m_text.setOrigin(localBoundsText.left + localBoundsText.width / 2.f,
		localBoundsText.top + localBoundsText.height / 2.f);
	m_text.setPosition(m_frame.getPosition());
}

void CharButton::setKeyCode(const int keyCode)
{
	if (keyCode != -1)
	{
		m_keyCode = keyCode;
		if (onChanged)
		{
			onChanged(m_keyCode);
		}
	}
	m_text.setString(keyToString(m_keyCode));
	updatePositionText();
}

void CharButton::resetText()
{
	m_text.setString("");
}

std::string CharButton::keyToString(const int keyCode) const
{
	string str;
	if (keyCode == 32)
	{
		str = "SPACE";
	}
	else if (keyCode >= 33 && keyCode <= 126)
	{
		char c = static_cast<char>(keyCode);
		c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
		str = string(1, c);
	}
	else
	{
		str = "?";
	}
	return str;
}

void CharButton::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_frame, states);
	target.draw(m_text, states);
}
