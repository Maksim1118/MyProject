#pragma once
#include "Scene.h"
#include "Button.h"
#include "SettingScene.h"
#include "ResourсeManager.h"

class SceneManager;
class MainScene : public Scene
{
public:
	MainScene(SceneManager *manager);
	~MainScene() = default;
	virtual void HandleEvent(Event& event, Vector2f& mousePos);
	virtual void draw(RenderWindow& window);
	virtual void TimeElapsed(int& diff);
private:
	Button button1 = Button(L"Начать игру", 100, 730);
	Button button2 = Button(L"Настройки", 100, 830);
	Button button3 = Button(L"Выход из игры", 100, 930);
	Button* current = nullptr;
	Sprite BackgroundSprite;
	SceneManager* _manager = nullptr;
	SettingScene* _SettingScene = nullptr;
};

