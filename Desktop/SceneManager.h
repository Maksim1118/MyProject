#pragma once
#include "Scene.h"

class GameScene;
class MainScene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();	
	Scene* GetCurrent();
	void SetMainScene();
	void SetGameScene(const std::string& playerName);
	void SetSettingScene();
private:
	MainScene* _MainScene = nullptr;
	GameScene* _GameScene = nullptr;
	Scene* _current = nullptr;
	bool _isSettingSceneVisible = false;
};

