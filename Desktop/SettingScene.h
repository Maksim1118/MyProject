//#pragma once
//#include "Scene.h"
//#include "Options.h"
//
//#include <list>
//#include <SFML/Audio.hpp>
//
//class SceneManager;
//class SettingScene : public Scene, public Options
//{
//public: 
//	SettingScene();
//	virtual void HandleEvent(Event& event, const Vector2f& mousePos) override;
//	virtual void TimeElapsed(int& diff);
//	virtual void draw(RenderWindow& window);
//};
//
//

#pragma once
#include "SubScene.h"
#include "HoverButton.h"
#include "CheckBox.h"
#include "VolumeSlider.h"
#include "CharButton.h"
#include "KeyBindController.h"
#include "TextButton.h"
#include "SettingConfig.h"
#include "ComboBox.h"

#include <memory>
#include <vector>


class SettingScene : public SubScene
{
public:
	SettingScene();
	virtual void HandleEvent(sf::Event& event, const sf::Vector2f& mousePos);
	virtual void TimeElapsed(int& diff, const sf::Vector2f& mousePos);
	virtual void draw(sf::RenderWindow& window);
private:
	sf::Sprite m_SpriteScene;

	std::unique_ptr<HoverButton> m_CloseButton;

	std::unique_ptr<CharButton> m_buttonFeed;

	std::unique_ptr<CharButton> m_buttonSplit;

	std::unique_ptr<TextButton> m_saveButton;

	std::unique_ptr<KeyBindController> m_keyBinder;

	std::vector<UI_Element*> m_elements;

	std::unique_ptr<CheckBox> m_checkBox;

	std::unique_ptr<VolumeSlider> m_volumeSlider;

	std::unique_ptr<ComboBox> m_comboBox;

	sf::RectangleShape m_selectedBg;

	static constexpr float SCENE_SCALE = 0.6f;
	static constexpr float SCENE_SIZE_X = 1000.f;
	static constexpr float SCENE_SIZE_Y = 1000.f;

	static constexpr float CLOSE_BUTTON_WIDTH = 120.f;
	static constexpr float CLOSE_BUTTON_HEIGHT = 50.f;

	static constexpr float CHECKBOX_SIZE = 70.f;
	static constexpr float CHECKBOX_X = 590.f;
	static constexpr float CHECKBOX_Y = 470.f;

	static constexpr float SLIDER_SIZE_X = 400.f;
	static constexpr float SLIDER_SIZE_Y = 30.f;
	static constexpr float SLIDER_X = 870.f;
	static constexpr float SLIDER_Y = 660.f;

	static constexpr unsigned int BUTTON_FEED_CHSIZE = 35.f;
	static constexpr float BUTTON_FEED_X = 690.f;
	static constexpr float BUTTON_FEED_Y = 1040.f;

	static constexpr unsigned int BUTTON_SPLIT_CHSIZE = 35.f;
	static constexpr float BUTTON_SPLIT_X = 600.f;
	static constexpr float BUTTON_SPLIT_Y = 850.f;

	static constexpr unsigned int BUTTON_SETTING_CHSIZE = 80;
	static constexpr float BUTTON_SETTING_X = 300.f;
	static constexpr float BUTTON_SETTING_Y = 1200.f;

	static constexpr float COMBOBOX_WIDTH = 300.f;
	static constexpr float COMBOBOX_HEIGHT = 50.f;
	static constexpr float COMBOBOX_X = 915.f;
	static constexpr float COMBOBOX_Y = 280.f;

	static constexpr float betweenItemOffset_Y = 70.f;
	static constexpr float selectedItemSize_Y = 120.f;
	static constexpr float startSelected_Y = 220.f;
	static constexpr float endSelected_Y = 1100.f;
};

