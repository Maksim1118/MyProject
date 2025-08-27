#include "Camera.h"

#include <iostream>

constexpr float minZoom = 0.3f;
constexpr float maxZoom = 1.3f;
constexpr float smoothing = 0.1f;
constexpr float scaleFactor = 100.f;
constexpr float logScale = 100.f;
constexpr float logOffset = 1.f;

constexpr float FieldMarginPercent = 0.15f;

constexpr int LogicW = 1200;
constexpr int LogicH = 1200;

namespace Server
{
	using namespace sf;
	using namespace std;


	Camera::Camera(sf::Vector2f fieldSize)
		:m_currZoom(minZoom),m_FieldSize(fieldSize)
	{

	}

	void Camera::setCenter(Vector2f center)
	{
		m_view.setCenter(center);
	}

	Vector2f Camera::getCenter() const
	{
		return m_view.getCenter();
	}

	Vector2i Camera::getLogicSize() const
	{
		/*return m_view.getSize();*/
		return {LogicW, LogicH };
	}

	void Camera::updateSize(Vector2f screenSize)
	{
		m_view.setSize(screenSize.x * m_currZoom, screenSize.y * m_currZoom);
	}

	sf::View Camera::getView() const
	{
		return m_view;
	}

	void Camera::limitZoomByFieldSize()
	{
		float marginW = m_FieldSize.x * FieldMarginPercent;
		float marginH = LogicH * FieldMarginPercent;

		float maxAllowedZoom = min((m_FieldSize.x - marginW) / LogicW, (m_FieldSize.y - marginH)/ LogicH);
		if (m_currZoom > maxAllowedZoom)
		{
			m_currZoom = maxAllowedZoom;
		}
	}

	float Camera::getZoom(float playerCurMass, float playerMinMass)
	{
		//float prop = (playerCurRadius - playerMinRadius) / (playerMaxRadius - playerMinRadius);
		//prop = clamp(prop, 0.f, 1.f);
		//float targZoom = minZoom + prop * (maxZoom - minZoom);

		//m_currZoom += (targZoom - m_currZoom) * smoothing;

		float diffMass = playerCurMass - playerMinMass;
		if (diffMass < 0.0f)
		{
			diffMass = 0.f;
		}
		float arg = diffMass / scaleFactor + logOffset;
		float prop = std::log(arg) / std::log(logScale);
		prop = std::clamp(prop, 0.f, 1.f);
		float targZoom = minZoom + prop * (maxZoom - minZoom);

		m_currZoom += (targZoom - m_currZoom) * smoothing;

		limitZoomByFieldSize();

		return m_currZoom;
	}

}
