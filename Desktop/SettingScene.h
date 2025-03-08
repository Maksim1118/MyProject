#pragma once
#include "Scene.h"
#include "Options.h"

#include <list>
#include <SFML/Audio.hpp>

class SceneManager;
class SettingScene : public Scene, public Options
{
public: 
	SettingScene();
	virtual void HandleEvent(Event& event, Vector2f& mousePos);
	virtual void TimeElapsed(int& diff);
	virtual void draw(RenderWindow& window);
};

