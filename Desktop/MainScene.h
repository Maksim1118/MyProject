#pragma once
#include "Scene.h"
#include "ButtonBackGround.h"
#include "SettingScene.h"
#include "ResourсeManager.h"


class SceneManager;
class SubScene;
class SettingScene;
class MainScene : public Scene
{
public:
	MainScene(SceneManager *manager);
	~MainScene() = default;
	virtual void HandleEvent(Event& event);
	virtual void draw(RenderWindow& window);
	virtual void TimeElapsed(int& diff);
private:
	class NickNameInput
	{
	public:
		NickNameInput();
		~NickNameInput() = default;
		void draw(sf::RenderTarget& target);
		void handleEvent(sf::Event& event, const Vector2f& mousePos);
		void TimeElapsed(int diff);
		std::string& getString();

	private:
		void setCursor();
		void updateBlinkCursor(int diff);
		void visibleCursorLock();
		void updateCursorPos();
	private:
		sf::Text text;
		std::string buffer;
		int prevBufferSize;
		bool isActive;
		bool cursorVisible;
		float cursorBlinkTime;

		sf::Sprite BackGroundInputField;
		sf::Sprite InputField;
		sf::RectangleShape cursor;
	};

private:
	NickNameInput* createInputName();
	void deleteInputName();

	std::unique_ptr<SubScene> createSettingScene();
	void deleteSettingScene();

	void lockButtons();
	void unlockButtons();
private:

	std::unique_ptr<SubScene> m_currSubScene;
	ButtonBackGround button1 = ButtonBackGround(L"Начать игру", 100, 730);
	ButtonBackGround button2 = ButtonBackGround(L"Настройки", 100, 830);
	ButtonBackGround button3 = ButtonBackGround(L"Выход из игры", 100, 930);
	ButtonBackGround* current = nullptr;
	Sprite BackgroundSprite;
	SceneManager* _manager = nullptr;
	NickNameInput* m_inputName;
};

