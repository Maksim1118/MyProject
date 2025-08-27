#include "VolumeSlider.h"

#include "ResourñeManager.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace sf;
using namespace std;

VolumeSlider::VolumeSlider(const Vector2f& pos, const Vector2f& size, const int volume, function<void(int)> onChanged)
	:trackTex(rec.textures[ResourceManager::Textures::TRACK])
	, thumbNormTex(rec.textures[ResourceManager::Textures::THUMB])
	, thumbHoveredTex(rec.textures[ResourceManager::Textures::THUMB_HOVERED])
	, m_onChanged(std::move(onChanged))
	, m_minX(0.f), m_maxX(0.f)
	, m_dragging(false)
	, m_value(0.f)
	, m_hovered(false)
{
	assert(trackTex && "trackTex must not be nullptr");
	assert(thumbNormTex && "thumbNormTex texture must not be nullptr");
	assert(thumbHoveredTex && "thumbHoveredTex texture must not be nullptr");

	m_trackSprite.setTexture(*trackTex);
	m_thumbSprite.setTexture(*thumbNormTex);

	m_thumbImage = thumbNormTex->copyToImage();
	setVolume(volume);
	setSize(size.x, size.y),
	setPosition(pos);
}

VolumeSlider::~VolumeSlider()
{
}

VolumeSlider::VolumeSlider(VolumeSlider&& other) noexcept
	: UI_Element(std::move(other))
	,trackTex(exchange(other.trackTex, nullptr))
	,thumbNormTex(exchange(other.thumbNormTex, nullptr))
	,thumbHoveredTex(exchange(other.thumbHoveredTex, nullptr))
	,m_trackSprite(std::move(other.m_trackSprite))
	,m_thumbSprite(std::move(other.m_thumbSprite))
	,m_thumbImage(std::move(other.m_thumbImage))
	,m_key_Feed(std::move(other.m_key_Feed))
	,m_value(other.m_value)
	,m_minX(other.m_minX)
	,m_maxX(other.m_maxX)
	,m_dragging(other.m_dragging)
	,m_hovered(other.m_hovered)
	,m_onChanged(std::move(other.m_onChanged))
{

}

VolumeSlider& VolumeSlider::operator=(VolumeSlider&& other) noexcept
{
	if (this != &other) 
	{
		UI_Element::operator=(std::move(other));
		trackTex = exchange(other.trackTex, nullptr);
		thumbNormTex = exchange(other.thumbNormTex, nullptr);
		thumbHoveredTex = exchange(other.thumbHoveredTex, nullptr);
		m_trackSprite = std::move(other.m_trackSprite);
		m_thumbSprite = std::move(other.m_thumbSprite);
		m_thumbImage = std::move(other.m_thumbImage);
		m_key_Feed = std::move(other.m_key_Feed);
		m_value = other.m_value;
		m_minX = other.m_minX;
		m_maxX = other.m_maxX;
		m_dragging = other.m_dragging;
		m_hovered = other.m_hovered;
		m_onChanged = std::move(other.m_onChanged);
	}
	return *this;
}

void VolumeSlider::handleEvent(const Event& event, const Vector2f& mousePos)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		if (isMouseOverThumb(mousePos))
		{
			m_dragging = true;
		}
	}
	else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		m_dragging = false;
	}
	else if (event.type == Event::MouseMoved && m_dragging)
	{
		Vector2f localPos = getInverseTransform().transformPoint(mousePos);

		float relativeX = localPos.x - m_trackSprite.getPosition().x;

		float clampedX = std::clamp(relativeX, m_minX, m_maxX);
		float percent = (clampedX - m_minX) / (m_maxX - m_minX);
		int newValue = static_cast<int>(percent * 100.f + 0.5f);
		setVolume(newValue);
		if (m_onChanged)
		{
			m_onChanged(m_value);
		}
	}
}

void VolumeSlider::update(const int diff, const sf::Vector2f& mousePos)
{
	if (m_dragging)
	{
		m_hovered = true;
		m_thumbSprite.setTexture(*thumbHoveredTex);
	}
	else
	{
		m_hovered = isMouseOverThumb(mousePos);
		m_thumbSprite.setTexture(m_hovered ? *thumbHoveredTex : *thumbNormTex);
	}
}

bool VolumeSlider::isBlocking()
{
	return false;
}

void VolumeSlider::setSize(const float width, const float height)
{
	float track_ScaleX = width / m_trackSprite.getTexture()->getSize().x;
	float track_ScaleY = height / m_trackSprite.getTexture()->getSize().y;
	m_trackSprite.setScale(track_ScaleX, track_ScaleY);

	float thumb_Scale = height * 2 / m_thumbSprite.getTexture()->getSize().y;
	m_thumbSprite.setScale(thumb_Scale, thumb_Scale);
	calcMinAndMax_X();
	positionAdjusment();
	updateThumbPosition();
}

void VolumeSlider::setVolume(const int volume)
{
	const int newVolume = clamp(volume, 0, 100);
	if (newVolume != m_value)
	{
		m_value = newVolume;
		updateThumbPosition();
	}
}

void VolumeSlider::calcMinAndMax_X()
{
	FloatRect trackLocalBounds = m_trackSprite.getGlobalBounds();
	m_minX = -trackLocalBounds.width / 2.f; 
	m_maxX = trackLocalBounds.width / 2.f;
}


void VolumeSlider::positionAdjusment()
{
	FloatRect trackLocalBounds = m_trackSprite.getLocalBounds();
	FloatRect thumbLocalBounds = m_thumbSprite.getLocalBounds();

	m_thumbSprite.setOrigin(thumbLocalBounds.width / 2.f, thumbLocalBounds.height / 2.f);
	m_trackSprite.setOrigin(trackLocalBounds.width / 2.f, trackLocalBounds.height / 2.f);

	m_thumbSprite.setPosition(m_trackSprite.getPosition());
}

bool VolumeSlider::isMouseOverThumb(const Vector2f& mousePos)
{
	FloatRect bounds = getTransform().transformRect(m_thumbSprite.getGlobalBounds());

	if (!bounds.contains(mousePos))
	{
		return false;
	}

	sf::Transform totalTransform = getTransform() * m_thumbSprite.getTransform();
	Vector2f localPos = totalTransform.getInverse().transformPoint(mousePos);

	if (localPos.x < 0 || localPos.y < 0 ||
		localPos.x >= static_cast<float>(m_thumbImage.getSize().x) ||
		localPos.y >= static_cast<float>(m_thumbImage.getSize().y))
	{
		return false;
	}
	Color pixel = m_thumbImage.getPixel(static_cast<unsigned int>(localPos.x), static_cast<unsigned int>(localPos.y));
	return pixel.a > 10;
}

void VolumeSlider::updateThumbPosition()
{
	float offset = m_value / 100.f;
	float newPosX = m_minX + offset * (m_maxX - m_minX);
	m_thumbSprite.setPosition(newPosX, m_thumbSprite.getPosition().y);
}

void VolumeSlider::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_trackSprite, states);
	target.draw(m_thumbSprite, states);
}
