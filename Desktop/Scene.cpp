#include "Scene.h"

Scene::Scene()
	:m_MousePos{0.f, 0.f}
{
}

void Scene::updateMousePos(const Vector2f& mousePos)
{
	m_MousePos = mousePos;
}

Scene::~Scene() = default;