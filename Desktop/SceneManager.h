#pragma once
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();	
	Scene* GetCurrent();
	void SetMainScene();
	void SetGameScene();
	void SetSettingScene();
private:
	Scene* _MainScene = nullptr;
	Scene* _GameScene  = nullptr;	
	Scene* _current = nullptr;
	bool _isSettingSceneVisible = false;
};

