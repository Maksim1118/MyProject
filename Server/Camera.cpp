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


	Camera::Camera(sf::Vector2f vSize)
		:m_viewSize(vSize)
	{

	}
}
