#include "SettingScene.h"
#include "SceneManager.h"

#include <SFML/Audio.hpp>

int Options::FeededKeyCode;

SettingScene::SettingScene()
{
	
}

void SettingScene::HandleEvent(Event& event, Vector2f& mousePos)
{
	if (event.type == Event::MouseMoved)
	{
		if (!_isKeySpliteChanged && PosKey(SpliteKeyCode, mousePos.x, mousePos.y))
		{
			SpliteKeyCode.setFillColor(Color::Red);
		}
		else
		{
			SpliteKeyCode.setFillColor(Color::Black);
		}
		if (!_isKeyFeedingChanged && PosKey(FeedingKeyCode, mousePos.x, mousePos.y))
		{
			FeedingKeyCode.setFillColor(Color::Red);
		}
		else
		{
			FeedingKeyCode.setFillColor(Color::Black);
		}
		if (_isSaveActive && PosButtonSave(mousePos.x, mousePos.y))
		{
			Save.setFillColor(Color::Red);
		}
		else if (!_isSaveActive)
		{
			Save.setFillColor(Color(150, 150, 150));
		}
		else  if (_isSaveActive)
		{
			Save.setFillColor(Color::Black);
		}
	}

	if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
	{
		if (PosSlider(mousePos.x , mousePos.y))
		{
			_isSliderPressed = true;
		}
		if (_isBoxPressed && PosBox(mousePos.x, mousePos.y))
		{
			_isBoxPressed = false;
			if (tempbool == _isBoxPressed)
				_isSaveActive = false;
		}
		else if (PosBox(mousePos.x, mousePos.y))
		{
			_isBoxPressed = true;
			if (tempbool == _isBoxPressed)
				_isSaveActive = false;
		}
		if (PosButtonClose(mousePos.x, mousePos.y))
		{
			_IsButtonClosePressed = true;			
		}
		
		if (!_isKeyFeedingChanged && PosKey(SpliteKeyCode, mousePos.x, mousePos.y))
		{			
			_isSaveActive = false;
			_isKeySpliteChanged = true;
			SpliteKeyCode.setString("");
		}
		if (!_isKeySpliteChanged && PosKey(FeedingKeyCode, mousePos.x, mousePos.y))
		{
			_isSaveActive = false;
			_isKeyFeedingChanged = true;
			FeedingKeyCode.setString("");
		}
		if (_isSaveActive && PosButtonSave(mousePos.x, mousePos.y))
		{
			Saved();
			tempvol = volume;
			tempbool = _isBoxPressed;
			_isSaveActive = false;
		}
	}

	if (event.type == Event::MouseButtonReleased && !Mouse::isButtonPressed(Mouse::Left))
	{
		_isSliderPressed = false;
	}
	if (_isSliderPressed && event.type == Event::MouseMoved)
	{
		moveSlider(event.mouseMove.x);
	}
	
	if (event.type == Event::KeyPressed)
	{	
		if (event.key.scancode == sf::Keyboard::Scan::Escape)
		{
		}
		if (_isKeySpliteChanged)
		{
			SplitedKeyCode = event.key.code;
			if (SplitedKeyCode != FeededKeyCode)
			{
				SpliteKeyCode = handleKeyPressed(SplitedKeyCode);
				SpliteKeyCode.setPosition(330.f + Setting_3.getGlobalBounds().width, 600.f);
				_isSaveActive = true;
				_isKeySpliteChanged = false;
			}
		
			
		}
		
		if (_isKeyFeedingChanged)
		{			
			FeededKeyCode = event.key.code;
			if (SplitedKeyCode != FeededKeyCode)
			{
				FeedingKeyCode = handleKeyPressed(FeededKeyCode);
				FeedingKeyCode.setPosition(330.f + Setting_4.getGlobalBounds().width, 700.f);
				_isSaveActive = true;
				_isKeyFeedingChanged = false;
			}		
		}
	}
}

void SettingScene::TimeElapsed(int& diff)
{
	if ((tempvol != volume || tempbool != _isBoxPressed) && !_isKeySpliteChanged && !_isKeyFeedingChanged)
	{
		_isSaveActive = true;
	}
	if (_isBoxPressed)
	{
		_CheckMark1 = CreateCheckMarkLine1();
		_CheckMark2 = CreateCheckMarkLine2();
	}
}
void SettingScene::draw(RenderWindow& window)
{
	window.draw(BackGround);
	window.draw(Setting);
	window.draw(Setting_1);
	window.draw(Setting_2);
	window.draw(Setting_3);
	window.draw(Setting_4);
	window.draw(Save);

	if (!_isKeySpliteChanged)
	{
		window.draw(SpliteKeyCode);
	}

	if (!_isKeyFeedingChanged)
	{
		window.draw(FeedingKeyCode);
	}
	window.draw(_LineMus);
	window.draw(_Regulator);
	window.draw(_Box);

	if (_isBoxPressed)
	{
		window.draw(_CheckMark1);
		window.draw(_CheckMark2);
	}
	window.draw(Close);
	window.draw(SP);
}
