#include "Camera.h"

//constexpr float minZoom = 0.3f;
//constexpr float maxZoom = 1.5f;
//constexpr float smoothing = 0.1f;
//constexpr float scaleFactor = 100.f;
//constexpr float logScale = 10.f;

constexpr float ZOOM_SPEED = 1e-4;



using namespace sf;
using namespace std;

Camera::Camera()
	:m_currZoom(0.f), m_targetZoom(0.f)
{

}

void Camera::setTargetZoom(float zoom)
{
	m_targetZoom = zoom;
}

void Camera::updateZoom(int& diff)
{
	float zoomDiff = m_targetZoom - m_currZoom;
	float zoomStep = ZOOM_SPEED * diff;
	if (abs(zoomStep) > abs(zoomDiff))
	{
		m_currZoom = m_targetZoom;
	}
	else
	{
		zoomStep = (zoomDiff > 0) ? zoomStep : -zoomStep;
		m_currZoom += zoomStep;
	}
	
}

void Camera::updateView(const Vector2f center)
{
	m_view.setSize({ 1200.f * m_currZoom, 1200.f * m_currZoom });
	m_view.setCenter(center);
}
const View Camera::getView() const
{
	return m_view;
}

//void Camera::setCenter(Vector2f center)
//{
//	m_view.setCenter(center);
//}
//
//Vector2f Camera::getCenter() const
//{
//	return m_view.getCenter();
//}
//
////Vector2f Camera::getSize() const
////{
////	/*return m_view.getSize();*/
////	
////}
//
//void Camera::updateSize(Vector2f screenSize)
//{
//	m_view.setSize(screenSize.x * m_currZoom, screenSize.y * m_currZoom);
//}
//
//sf::View Camera::getView() const
//{
//	return m_view;
//}
//
//void Camera::updateZoom(float diff, float playerCurRadius, float playerMinRadius)
//{
//	//float prop = (playerCurRadius - playerMinRadius) / (playerMaxRadius - playerMinRadius);
//	//prop = clamp(prop, 0.f, 1.f);
//	//float targZoom = minZoom + prop * (maxZoom - minZoom);
//
//	//m_currZoom += (targZoom - m_currZoom) * smoothing;
//
//	float diffRadius = playerCurRadius - playerMinRadius;
//	float prop = std::log(diffRadius/scaleFactor + 1.f) / std::log(logScale);
//	prop = std::clamp(prop, 0.f, 1.f);
//	float targZoom = minZoom + prop * (maxZoom - minZoom);
//
//	m_currZoom += (targZoom - m_currZoom) * smoothing;
//}
