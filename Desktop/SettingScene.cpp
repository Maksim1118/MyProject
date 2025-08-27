//#include "SettingScene.h"
//#include "SceneManager.h"
//
//#include <SFML/Audio.hpp>
//
//int Options::FeededKeyCode;
//
//SettingScene::SettingScene()
//{
//	
//}
//
//void SettingScene::HandleEvent(Event& event, const Vector2f& mousePos)
//{
//	if (event.type == Event::MouseMoved)
//	{
//		if (!_isKeySpliteChanged && PosKey(SpliteKeyCode, mousePos.x, mousePos.y))
//		{
//			SpliteKeyCode.setFillColor(Color::Red);
//		}
//		else
//		{
//			SpliteKeyCode.setFillColor(Color::Black);
//		}
//		if (!_isKeyFeedingChanged && PosKey(FeedingKeyCode, mousePos.x, mousePos.y))
//		{
//			FeedingKeyCode.setFillColor(Color::Red);
//		}
//		else
//		{
//			FeedingKeyCode.setFillColor(Color::Black);
//		}
//		if (_isSaveActive && PosButtonSave(mousePos.x, mousePos.y))
//		{
//			Save.setFillColor(Color::Red);
//		}
//		else if (!_isSaveActive)
//		{
//			Save.setFillColor(Color(150, 150, 150));
//		}
//		else  if (_isSaveActive)
//		{
//			Save.setFillColor(Color::Black);
//		}
//	}
//
//	if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
//	{
//		if (PosSlider(mousePos.x , mousePos.y))
//		{
//			_isSliderPressed = true;
//		}
//		if (_isBoxPressed && PosBox(mousePos.x, mousePos.y))
//		{
//			_isBoxPressed = false;
//			if (tempbool == _isBoxPressed)
//				_isSaveActive = false;
//		}
//		else if (PosBox(mousePos.x, mousePos.y))
//		{
//			_isBoxPressed = true;
//			if (tempbool == _isBoxPressed)
//				_isSaveActive = false;
//		}
//		if (PosButtonClose(mousePos.x, mousePos.y))
//		{
//			_IsButtonClosePressed = true;			
//		}
//		
//		if (!_isKeyFeedingChanged && PosKey(SpliteKeyCode, mousePos.x, mousePos.y))
//		{			
//			_isSaveActive = false;
//			_isKeySpliteChanged = true;
//			SpliteKeyCode.setString("");
//		}
//		if (!_isKeySpliteChanged && PosKey(FeedingKeyCode, mousePos.x, mousePos.y))
//		{
//			_isSaveActive = false;
//			_isKeyFeedingChanged = true;
//			FeedingKeyCode.setString("");
//		}
//		if (_isSaveActive && PosButtonSave(mousePos.x, mousePos.y))
//		{
//			Saved();
//			tempvol = volume;
//			tempbool = _isBoxPressed;
//			_isSaveActive = false;
//		}
//	}
//
//	if (event.type == Event::MouseButtonReleased && !Mouse::isButtonPressed(Mouse::Left))
//	{
//		_isSliderPressed = false;
//	}
//	if (_isSliderPressed && event.type == Event::MouseMoved)
//	{
//		moveSlider(event.mouseMove.x);
//	}
//	
//	if (event.type == Event::KeyPressed)
//	{	
//		if (event.key.scancode == sf::Keyboard::Scan::Escape)
//		{
//		}
//		if (_isKeySpliteChanged)
//		{
//			SplitedKeyCode = event.key.code;
//			if (SplitedKeyCode != FeededKeyCode)
//			{
//				SpliteKeyCode = handleKeyPressed(SplitedKeyCode);
//				SpliteKeyCode.setPosition(330.f + Setting_3.getGlobalBounds().width, 600.f);
//				_isSaveActive = true;
//				_isKeySpliteChanged = false;
//			}
//		
//			
//		}
//		
//		if (_isKeyFeedingChanged)
//		{			
//			FeededKeyCode = event.key.code;
//			if (SplitedKeyCode != FeededKeyCode)
//			{
//				FeedingKeyCode = handleKeyPressed(FeededKeyCode);
//				FeedingKeyCode.setPosition(330.f + Setting_4.getGlobalBounds().width, 700.f);
//				_isSaveActive = true;
//				_isKeyFeedingChanged = false;
//			}		
//		}
//	}
//}
//
//void SettingScene::TimeElapsed(int& diff)
//{
//	if ((tempvol != volume || tempbool != _isBoxPressed) && !_isKeySpliteChanged && !_isKeyFeedingChanged)
//	{
//		_isSaveActive = true;
//	}
//	if (_isBoxPressed)
//	{
//		_CheckMark1 = CreateCheckMarkLine1();
//		_CheckMark2 = CreateCheckMarkLine2();
//	}
//}
//void SettingScene::draw(RenderWindow& window)
//{
//	window.draw(BackGround);
//	window.draw(Setting);
//	window.draw(Setting_1);
//	window.draw(Setting_2);
//	window.draw(Setting_3);
//	window.draw(Setting_4);
//	window.draw(Save);
//
//	if (!_isKeySpliteChanged)
//	{
//		window.draw(SpliteKeyCode);
//	}
//
//	if (!_isKeyFeedingChanged)
//	{
//		window.draw(FeedingKeyCode);
//	}
//	window.draw(_LineMus);
//	window.draw(_Regulator);
//	window.draw(_Box);
//
//	if (_isBoxPressed)
//	{
//		window.draw(_CheckMark1);
//		window.draw(_CheckMark2);
//	}
//	window.draw(Close);
//	window.draw(SP);
//}
//

#include "SettingScene.h"
#include "ResourñeManager.h"

#include "SimplifiedSfmlFunc.h"
#include "Options.h"

constexpr float y_menuItemSize = 70.f;
constexpr int numFields = 5;

Color selectedColor = Color(80, 80, 80);

SettingScene::SettingScene()
	: m_keyBinder(make_unique<KeyBindController>())
{
	auto& sceneTexture = *rec.textures[ResourceManager::Textures::SETTINGBG];
	sceneTexture.setSmooth(true);
	Vector2f origin = { sceneTexture.getSize().x / 2.f, sceneTexture.getSize().y / 2.f };
	Vector2f pos = { 1200.f / 2.f, 1200.f / 2.f };
	SFML_SIMPLIFIED::initSprite(m_SpriteScene, sceneTexture, origin, pos, { SCENE_SIZE_X, SCENE_SIZE_Y });

	const Vector2f sceneScale = m_SpriteScene.getScale();

	const FloatRect sceneBounds = m_SpriteScene.getGlobalBounds();

	float posCloseButtonX = sceneBounds.left + sceneBounds.width - CLOSE_BUTTON_WIDTH * sceneScale.x;
	float posCloseButtonY = sceneBounds.top;

	m_CloseButton  = make_unique<HoverButton>(
		Vector2f{ posCloseButtonX, posCloseButtonY }, 
		Vector2f{ CLOSE_BUTTON_WIDTH * sceneScale.x, CLOSE_BUTTON_HEIGHT * sceneScale.y } 
	);
	m_CloseButton->onClick = [this]() {m_shouldClose = true; };
	m_elements.push_back(m_CloseButton.get());

	SettingConfig localConfig = Options::getInstance().getSettingConfig();

	//const auto& boxTex = *rec.textures[ResourceManager::Textures::BOX];
	//const auto& checkMarkTex = *rec.textures[ResourceManager::Textures::CHECKMARK];
	const float boxPosX = sceneBounds.left + sceneScale.x * CHECKBOX_X;
	const float boxPosY = sceneBounds.top + sceneScale.y * CHECKBOX_Y;

	m_checkBox = std::make_unique<CheckBox>(
		Vector2f{ boxPosX, boxPosY },
		CHECKBOX_SIZE * sceneScale.y,
		localConfig.m_fullScreen,
		[](bool flag) { Options::getInstance().setFullScreen(flag); }
	);
	m_elements.push_back(m_checkBox.get());

	float sliderPosX = sceneBounds.left + sceneScale.x * SLIDER_X;
	float sliderPosY = sceneBounds.top + sceneScale.y * SLIDER_Y;

	m_volumeSlider = make_unique <VolumeSlider>(
		Vector2f{ sliderPosX, sliderPosY },
		Vector2f{ SLIDER_SIZE_X * sceneScale.x, SLIDER_SIZE_Y * sceneScale.y },
		localConfig.m_settingVolume,
		[](int volume)
		{
			Options::getInstance().setVolume(volume);
		}
	);
	m_elements.push_back(m_volumeSlider.get());

	
	const float buttonFeedPosX = sceneBounds.left + sceneScale.x * BUTTON_FEED_X;
	const float buttonFeedPosY = sceneBounds.top + sceneScale.y * BUTTON_FEED_Y;

	m_buttonFeed = make_unique<CharButton>(
		localConfig.m_key_Feed, 
		*rec.font,
		BUTTON_FEED_CHSIZE, 
		Vector2f{buttonFeedPosX, buttonFeedPosY},
		[](int keyCode) {Options::getInstance().setKeyCode(KeyType::FEED, keyCode); });
	m_buttonFeed->onClick = [this]() { if (m_keyBinder) { m_keyBinder->Bind(m_buttonFeed.get()); } };
	m_elements.push_back(m_buttonFeed.get());


	const float buttonSplitPosX = sceneBounds.left + sceneScale.x * BUTTON_SPLIT_X;
	const float buttonSplitPosY = sceneBounds.top + sceneScale.y * BUTTON_SPLIT_Y;

	m_buttonSplit = make_unique<CharButton>(
		localConfig.m_key_Split, 
		*rec.font, 
		BUTTON_SPLIT_CHSIZE, 
		Vector2f{buttonSplitPosX, buttonSplitPosY},
		[](int keyCode) {Options::getInstance().setKeyCode(KeyType::SPLIT, keyCode); });
	m_buttonSplit->onClick = [this]() { if (m_keyBinder) { m_keyBinder->Bind(m_buttonSplit.get()); } };
	m_elements.push_back(m_buttonSplit.get());


	float buttonSettingPosX = sceneBounds.left + sceneScale.x * BUTTON_SETTING_X;
	float buttonSettingPosY = sceneBounds.top + sceneScale.y * BUTTON_SETTING_Y;

	m_saveButton = make_unique<TextButton>(
		"save",
		*rec.font,
		Vector2f{ buttonSettingPosX, buttonSettingPosY },
		BUTTON_SETTING_CHSIZE,
		[]() { Options::getInstance().applySetting(); }
	);

	m_elements.push_back(m_saveButton.get());


	vector<string> listString;
	for (const auto& item : listResolutions)
	{
		listString.push_back(item.second);
	}

	const float ComboBoxPosX = sceneBounds.left + sceneScale.x * COMBOBOX_X;
	const float ComboBoxPosY = sceneBounds.top + sceneScale.y * COMBOBOX_Y;

	m_comboBox = std::make_unique<ComboBox>(
		*rec.font, 
		listString, 
		0,
		Vector2f(COMBOBOX_WIDTH, COMBOBOX_HEIGHT),
		Vector2f(ComboBoxPosX, ComboBoxPosY),
		[](string& str) { Options::getInstance().setResolution(str); });
	m_comboBox->setOrigin(COMBOBOX_WIDTH / 2.f, COMBOBOX_HEIGHT / 2.f);

	m_elements.push_back(m_comboBox.get());
}

void SettingScene::HandleEvent(Event& event, const sf::Vector2f& mousePos)
{
	auto blockingIt = std::find_if(m_elements.begin(), m_elements.end(),
		[](const auto& elem) { return elem->isBlocking(); });

	if (blockingIt != m_elements.end())
	{
		(*blockingIt)->handleEvent(event, mousePos);
	}
	else
	{
		for (auto& elem : m_elements)
		{
			elem->handleEvent(event, mousePos);
		}
	}
	m_keyBinder->handleEvent(event);
}

void SettingScene::TimeElapsed(int& diff, const sf::Vector2f& mousePos)
{
	if (Options::getInstance().hasChanged())
	{
		m_saveButton->setEnable(true);
	}
	else
	{
		m_saveButton->setEnable(false);
	}

	auto blockingIt = std::find_if(m_elements.begin(), m_elements.end(),
		[](const auto& elem) { return elem->isBlocking(); });

	if (blockingIt != m_elements.end())
	{
		(*blockingIt)->update(diff, mousePos);
	}
	else
	{
		for (auto& elem : m_elements)
		{
			elem->update(diff, mousePos);
		}
	}
}

void SettingScene::draw(RenderWindow& window)
{
	window.draw(m_SpriteScene);
	for (const auto& elem : m_elements)
	{
		window.draw(*elem);
	}
}
