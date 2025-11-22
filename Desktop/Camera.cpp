#include "Camera.h"

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
