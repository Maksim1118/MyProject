#include "SceneManager.h"
#include "MainScene.h"
#include "GameScene.h"
#include "SettingScene.h"


SceneManager::SceneManager()
{
	_GameScene = new GameScene(this);
	_MainScene = new MainScene(this);

	_current = _MainScene;
}

SceneManager::~SceneManager()
{
	delete _MainScene;
	delete _GameScene;
}

Scene* SceneManager::GetCurrent()
{
	return _current;
}

void SceneManager::SetMainScene()
{
	_current = _MainScene;	
}

void SceneManager::SetGameScene(const string& playerName)
{
	_GameScene->setActive(playerName);
	_current = _GameScene;
}
