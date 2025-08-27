#pragma once

#include <SFML/Graphics.hpp>

class SubScene
{
public:
	SubScene();
	SubScene(const SubScene&) = delete;
	SubScene& operator=(const SubScene&) = delete;
	SubScene(const SubScene&&) = delete;
	SubScene& operator=(const SubScene&&) = delete;
	virtual void HandleEvent(sf::Event& event, const sf::Vector2f& mousePos) = 0;
	virtual void TimeElapsed(int& diff, const sf::Vector2f& mousePos) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	bool shouldClose() const;
	virtual ~SubScene() = default;
protected:
	bool m_shouldClose;
};

inline bool SubScene::shouldClose() const
{
	return m_shouldClose;
}
