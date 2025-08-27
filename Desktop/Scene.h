#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
class Scene 
{	
public:
	Scene();
	virtual void HandleEvent(Event &event) = 0;
	virtual void TimeElapsed(int& diff) = 0;
	virtual void draw(RenderWindow& window) = 0;
	void updateMousePos(const Vector2f& mousePos);
	virtual ~Scene()=0;

protected:
	sf::Vector2f m_MousePos;
};

