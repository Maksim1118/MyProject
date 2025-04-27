#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
class Scene 
{	
public:
	virtual void HandleEvent(Event &event, Vector2f& mousePos) = 0;
	virtual void TimeElapsed(int& diff) = 0;
	virtual void setActive();
	virtual void draw(RenderWindow& window) = 0;
	virtual ~Scene()=0;
};

