#pragma once

#include "Ui_Element.h"

#include <functional>

class VolumeSlider: public UI_Element
{
public:
	VolumeSlider(const sf::Vector2f& pos, const sf::Vector2f& size, const int volume, std::function<void(int)> onChanged);
	~VolumeSlider()override;

	VolumeSlider(VolumeSlider&&) noexcept;
	VolumeSlider& operator=(VolumeSlider&&) noexcept;


	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)override;
	void update(const int diff, const sf::Vector2f& mousePos)override;
	bool isBlocking() override;
	void setSize(const float width, const float height);
private:
	void calcMinAndMax_X();
	void positionAdjusment();
	bool isMouseOverThumb(const sf::Vector2f& mousePos);
	void updateThumbPosition();
	void setVolume(const int volume);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const sf::Texture* trackTex;
	const sf::Texture* thumbNormTex;
	const sf::Texture* thumbHoveredTex;

	sf::Sprite m_trackSprite;
	sf::Sprite m_thumbSprite;
	sf::Image m_thumbImage;
	sf::Text m_key_Feed;
	int m_value;
	float m_minX;
	float m_maxX;
	bool m_dragging;
	bool m_hovered;
	std::function<void(int)> m_onChanged;
};