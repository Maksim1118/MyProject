#pragma once
#include "Scene.h"
#include "Food.h"
#include "Hero.h"
#include "Thorn.h"
#include "SettingScene.h"
#include "Objects.h"
//#include "GameEngine.h"
#include "ResourñeManager.h"
constexpr int ArrFoodSize = 500;
constexpr int SetPosFood = 1190;
constexpr int SetPosThorn = 1100;
class SceneManager;

class GameScene : public Scene
{
public:
	GameScene(SceneManager* manager);
	virtual void HandleEvent(Event& event, Vector2f& mousePos);
	virtual void draw(RenderWindow& window);
	bool isCollWithMap(const Vector2f& pos);
	Vector2f getCoorCollWithMap(const Vector2f& pos);
	virtual void TimeElapsed(int& diff);
	Vector2f getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height);
	void setActive() override;
	
private:
	SceneManager *_manager= nullptr;
	Sprite fieldBGSprite;
	bool _GameOver = false;
	Text text;
	vector<shared_ptr<Objects>> objects;
	int requestTimeStart = 0;
	float scale = 1.00f;
	Vector2f CameraPos;
	shared_ptr<Hero> _hero;
	Vector2f posMouse{ 0.f,0.f };
	int timer = 0;
};

