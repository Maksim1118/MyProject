#include "Options.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "ResourñeManager.h"

float Options::volume = 0;

float Options::_WindowWidth = 0;
float Options::_WindowHeight =  0;

Options::Options()
{
	tempbool = _isBoxPressed;

	BackGround.setSize(Vector2f(800.f, 800.f));
	BackGround.setPosition(200.f, 200.f);
	BackGround.setFillColor(Color(204, 204, 204));
	
	Close.setTexture(*rec.textures[ResourceManager::Textures::BUTCLOSE]);
	Close.setPosition(200.f + (BackGround.getGlobalBounds().width - Close.getGlobalBounds().width), 200.f);

	SP.setTexture(*rec.textures[ResourceManager::Textures::SETTINGPICT]);
	SP.setPosition(BackGround.getGlobalBounds().width, 950.f - SP.getGlobalBounds().height);

	Setting = CreateTitle(L"ÍÀÑÒÐÎÉÊÈ", 40);
	Setting.setPosition((1200.f - Setting.getGlobalBounds().width) / 2, 250.f);

	Setting_1 = CreateTitle(L"Íà Âåñü Ýêðàí:", 25);
	Setting_1.setPosition(300.f, 400.f);

	Setting_2 = CreateTitle(L"Ãðîìêîñòü Ìóçûêè:", 25);
	Setting_2.setPosition(300.f, 500.f);

	Setting_3 = CreateTitle(L"Äåëåíèå:", 25);
	Setting_3.setPosition(300.f, 600.f);


	Setting_4 = CreateTitle(L"Ïðèêîðìêà:", 25);
	Setting_4.setPosition(300.f, 700.f);

	Save = CreateTitle(L"ÑÎÕÐÀÍÈÒÜ", 25);
	Save.setPosition(BackGround.getPosition().x + 100.f, BackGround.getPosition().y + (BackGround.getGlobalBounds().height - 100.f));

	SpliteKeyCode = handleKeyPressed(SplitedKeyCode);
	SpliteKeyCode.setPosition(330.f + Setting_3.getGlobalBounds().width, 600.f);
	
	FeedingKeyCode = handleKeyPressed(FeededKeyCode);
	FeedingKeyCode.setPosition(330.f + Setting_4.getGlobalBounds().width, 700.f);
	
	_LineMus = CreateLineMus();
	_Regulator = CreateRegulator();
	_Box = CreateCheckBox();
}

RectangleShape Options::CreateLineMus()
{
	RectangleShape line_mus;
	line_mus.setSize(Vector2f(2.f, 200.f));
	line_mus.setPosition((350.f + Setting_2.getGlobalBounds().width), 505.f + Setting_2.getGlobalBounds().height / 2);
	line_mus.setRotation(270.f);
	return  line_mus;
}

RectangleShape Options::CreateRegulator()
{
	rec.music->setVolume(volume);
	tempvol = volume;

	RectangleShape regulator;
	regulator.setSize(Vector2f(6.f, 30.f));
	regulator.setFillColor(Color(90, 90, 90));
	regulator.setOutlineThickness(2.f);
	regulator.setOutlineColor(Color::Red);
	regulator.setPosition(_LineMus.getPosition().x + _LineMus.getGlobalBounds().width / 100.f * rec.music->getVolume(), _LineMus.getPosition().y - 30.f / 2);
	return regulator;
}

void Options::moveSlider(int& mouseX)
{
    if (mouseX > _LineMus.getPosition().x && mouseX < _LineMus.getPosition().x + _LineMus.getGlobalBounds().width - 4.f)
	{
		_Regulator.setPosition(mouseX , _Regulator.getPosition().y);
	}
	else if (mouseX <= _LineMus.getPosition().x)
	{
		_Regulator.setPosition(_LineMus.getPosition().x + 2.f, _Regulator.getPosition().y);
	}
	else if (mouseX >= _LineMus.getPosition().x + _LineMus.getGlobalBounds().width - 4.f)
	{
		_Regulator.setPosition(_LineMus.getPosition().x + _LineMus.getGlobalBounds().width - (_Regulator.getGlobalBounds().width - 4.f), _Regulator.getPosition().y);
	}
	float  K = (200.f - 8.f) / 100.f;
	volume = ((_Regulator.getPosition().x - 2.f) - _LineMus.getPosition().x) / K;	
	if (volume < 0)
		volume = 0;
	rec.music->setVolume(volume);
}

bool Options::PosSlider(const float& x, const float& y)
{
	float outlineThickness = _Regulator.getOutlineThickness();
	if (x < _Regulator.getPosition().x - outlineThickness)
		return false;
	if (x > _Regulator.getPosition().x + _Regulator.getGlobalBounds().width + outlineThickness)
		return false;
	if (y < _Regulator.getPosition().y - outlineThickness)
		return false;
	if (y > _Regulator.getPosition().y + _Regulator.getGlobalBounds().height + outlineThickness)
		return false;
	return true;
}

bool Options::PosBox(const float& x, const float&  y)
{
	if (x <= _Box.getPosition().x)
		return false;
	if (x >= _Box.getPosition().x + _Box.getGlobalBounds().width)
		return false;
	if (y <= _Box.getPosition().y)
		return false;
	if (y >= _Box.getPosition().y + _Box.getGlobalBounds().height)
		return false;
	return true;
}

bool Options::PosKey(const Text& text, const float& x, const float&  y)
{
	if (x <= text.getGlobalBounds().left)
		return false;
	if (x >= text.getGlobalBounds().left + text.getGlobalBounds().width)
		return false;
	if (y <= text.getGlobalBounds().top)
		return false;
	if (y >= text.getGlobalBounds().top + text.getGlobalBounds().height)
		return false;
	return true;
}

bool Options::PosButtonClose(const float& x, const float& y)
{
	if (x <= Close.getPosition().x)
		return false;
	if (x >= Close.getPosition().x + Close.getGlobalBounds().width)
		return false;
	if (y <= Close.getPosition().y)
		return false;
	if (y >= Close.getPosition().y + Close.getGlobalBounds().height)
		return false;
	return true;
}

bool Options::PosButtonSave(const float& x, const float& y)
{
	if (x < Save.getPosition().x)
		return false;
	if (x > Save.getPosition().x + Save.getGlobalBounds().width)
		return false;
	if (y < Save.getPosition().y)
		return false;
	if (y > Save.getPosition().y + Save.getGlobalBounds().height)
		return false;
	return true;
}

void Options::Saved()
{
	ofstream File;
	File.open("SaveSettings.txt");
	File << volume << " " << SplitedKeyCode << " " << FeededKeyCode  << " " << _isBoxPressed;
	File.close();
}
void Options::OpenSaved(const char* file)
{
	ifstream _file(file);
	if (!_file.is_open())
	{
	    volume = 30.f;
		SplitedKeyCode = Keyboard::Key::Space;
		FeededKeyCode = Keyboard::Key::E;
	}
	_file >> volume >> SplitedKeyCode >> FeededKeyCode >> _isBoxPressed;
}

RectangleShape Options::CreateCheckBox()
{
	RectangleShape Square;
	Square.setSize(Vector2f(20.f, 20.f));
	Square.setPosition(330.f + Setting_1.getGlobalBounds().width, 406.f);
	Square.setFillColor(Color::Transparent);
	Square.setOutlineThickness(2.f);
	Square.setOutlineColor(Color::Black);
	return Square;
}
RectangleShape Options::CreateCheckMarkLine1()
{
	RectangleShape CheckMark(Vector2f(3.f, 25.f));
	CheckMark.setPosition(_Box.getPosition().x + 20.f / 2, _Box.getPosition().y + 20.f / 2);
	CheckMark.setFillColor(Color::Black);
	CheckMark.rotate(150.f);
	return CheckMark;
}
RectangleShape Options::CreateCheckMarkLine2()
{
	RectangleShape CheckMark(Vector2f(3.f, 40.f));
	CheckMark.setPosition(_Box.getPosition().x + 20.f / 2, _Box.getPosition().y + 20.f / 2);
	CheckMark.setFillColor(Color::Black);
	CheckMark.rotate(210.f);
	return CheckMark;
}
Text Options::handleKeyPressed(int& code)
{
	Text key;
	switch (code)
	{
	case Keyboard::A:
		return  key = CreateTitle(L"A", 25);
	case Keyboard::B:
		return  key = CreateTitle(L"B", 25);
	case Keyboard::C:
		return  key = CreateTitle(L"C", 25);
	case Keyboard::D:
		return  key = CreateTitle(L"D", 25);
	case Keyboard::E:
		return  key = CreateTitle(L"E", 25);
	case Keyboard::F:
		return  key = CreateTitle(L"F", 25);
	case Keyboard::G:
		return  key = CreateTitle(L"G", 25);
	case Keyboard::H:
		return  key = CreateTitle(L"H", 25);
	case Keyboard::I:
		return  key = CreateTitle(L"I", 25);
	case Keyboard::J:
		return  key = CreateTitle(L"J", 25);
	case Keyboard::K:
		return  key = CreateTitle(L"K", 25);
	case Keyboard::L:
		return  key = CreateTitle(L"L", 25);
	case Keyboard::M:
		return  key = CreateTitle(L"M", 25);
	case Keyboard::N:
		return  key = CreateTitle(L"N", 25);
	case Keyboard::O:
		return  key = CreateTitle(L"O", 25);
	case Keyboard::P:
		return  key = CreateTitle(L"P", 25);
	case Keyboard::Q:
		return  key = CreateTitle(L"Q", 25);
	case Keyboard::R:
		return  key = CreateTitle(L"R", 25);
	case Keyboard::S:
		return  key = CreateTitle(L"S", 25);
	case Keyboard::T:
		return  key = CreateTitle(L"T", 25);
	case Keyboard::U:
		return  key = CreateTitle(L"U", 25);
	case Keyboard::V:
		return  key = CreateTitle(L"V", 25);
	case Keyboard::W:
		return  key = CreateTitle(L"W", 25);
	case Keyboard::X:
		return  key = CreateTitle(L"X", 25);
	case Keyboard::Y:
		return  key = CreateTitle(L"Y", 25);
	case Keyboard::Z:
		return  key = CreateTitle(L"Z", 25);
	case Keyboard::Num1:
		return  key = CreateTitle(L"Num1", 25);
	case Keyboard::Num2:
		return  key = CreateTitle(L"Num2", 25);
	case Keyboard::Num3:
		return  key = CreateTitle(L"Num3", 25);
	case Keyboard::Num4:
		return  key = CreateTitle(L"Num4", 25);
	case Keyboard::Num5:
		return  key = CreateTitle(L"Num5", 25);
	case Keyboard::Num6:
		return  key = CreateTitle(L"Num6", 25);
	case Keyboard::Num7:
		return  key = CreateTitle(L"Num7", 25);
	case Keyboard::Num8:
		return  key = CreateTitle(L"Num8", 25);
	case Keyboard::Num9:
		return  key = CreateTitle(L"Num9", 25);
	case Keyboard::Num0:
		return  key = CreateTitle(L"Num0", 25);
	case Keyboard::Enter:
		return  key = CreateTitle(L"Enter", 25);
	case Keyboard::BackSpace:
		return  key = CreateTitle(L"BackSpace", 25);
	case Keyboard::Tab:
		return  key = CreateTitle(L"Tab", 25);
	case Keyboard::Space:
		return  key = CreateTitle(L"Space", 25);
	case Keyboard::Hyphen:
		return  key = CreateTitle(L"Hyphen", 25);
	case Keyboard::Equal:
		return  key = CreateTitle(L"Equal", 25);
	case Keyboard::LBracket:
		return  key = CreateTitle(L"LBracket", 25);
	case Keyboard::RBracket:
		return  key = CreateTitle(L"RBracket", 25);
	case Keyboard::Backslash:
		return  key = CreateTitle(L"Backslash", 25);
	case Keyboard::Semicolon:
		return  key = CreateTitle(L"Semicolon", 25);
	case Keyboard::Apostrophe:
		return  key = CreateTitle(L"Apostrophe", 25);
	case Keyboard::Grave:
		return  key = CreateTitle(L"Grave", 25);
	case Keyboard::Comma:
		return  key = CreateTitle(L"Comma", 25);
	case Keyboard::Period:
		return  key = CreateTitle(L"Period", 25);
	case Keyboard::Slash:
		return  key = CreateTitle(L"Slash", 25);
	case Keyboard::LShift:
		return  key = CreateTitle(L"LShift", 25);
	case Keyboard::LControl:
		return  key = CreateTitle(L"LControl", 25);
	case Keyboard::LAlt:
		return  key = CreateTitle(L"LAlt", 25);
	case Keyboard::LSystem:
		return  key = CreateTitle(L"LSystem", 25);
	case Keyboard::RControl:
		return  key = CreateTitle(L"RControl", 25);
	case Keyboard::RShift:
		return  key = CreateTitle(L"RShift", 25);
	case Keyboard::RAlt:
		return  key = CreateTitle(L"RAlt", 25);
	case Keyboard::RSystem:
		return  key = CreateTitle(L"RSystem", 25);
	case Keyboard::Menu:
		return  key = CreateTitle(L"Menu", 25);
	case Keyboard::PageUp:
		return  key = CreateTitle(L"PageUp", 25);
	case Keyboard::PageDown:
		return  key = CreateTitle(L"PageDown", 25);
	case Keyboard::End:
		return  key = CreateTitle(L"End", 25);
	case Keyboard::Home:
		return  key = CreateTitle(L"Home", 25);
	case Keyboard::Insert:
		return  key = CreateTitle(L"Insert", 25);
	case Keyboard::Delete:
		return  key = CreateTitle(L"Delete", 25);
	case Keyboard::Add:
		return  key = CreateTitle(L"Add", 25);
	case Keyboard::Subtract:
		return  key = CreateTitle(L"Subtract", 25);
	case Keyboard::Multiply:
		return  key = CreateTitle(L"Multiply", 25);
	case Keyboard::Divide:
		return  key = CreateTitle(L"Divide", 25);
	case Keyboard::Left:
		return  key = CreateTitle(L"Left", 25);
	case Keyboard::Right:
		return  key = CreateTitle(L"Right", 25);
	case Keyboard::Up:
		return  key = CreateTitle(L"Up", 25);
	case Keyboard::Down:
		return  key = CreateTitle(L"Down", 25);
	case Keyboard::Numpad0:
		return  key = CreateTitle(L"Numpad0", 25);
	case Keyboard::Numpad1:
		return  key = CreateTitle(L"Numpad1", 25);
	case Keyboard::Numpad2:
		return  key = CreateTitle(L"Numpad2", 25);
	case Keyboard::Numpad3:
		return  key = CreateTitle(L"Numpad3", 25);
	case Keyboard::Numpad4:
		return  key = CreateTitle(L"Numpad4", 25);
	case Keyboard::Numpad5:
		return  key = CreateTitle(L"Numpad5", 25);
	case Keyboard::Numpad6:
		return  key = CreateTitle(L"Numpad6", 25);
	case Keyboard::Numpad7:
		return  key = CreateTitle(L"Numpad7", 25);
	case Keyboard::Numpad8:
		return  key = CreateTitle(L"Numpad8", 25);
	case Keyboard::Numpad9:
		return  key = CreateTitle(L"Numpad9", 25);
	case Keyboard::F1:
		return  key = CreateTitle(L"F1", 25);
	case Keyboard::F2:
		return  key = CreateTitle(L"F2", 25);
	case Keyboard::F3:
		return  key = CreateTitle(L"F3", 25);
	case Keyboard::F4:
		return  key = CreateTitle(L"F4", 25);
	case Keyboard::F5:
		return  key = CreateTitle(L"F5", 25);
	case Keyboard::F6:
		return  key = CreateTitle(L"F6", 25);
	case Keyboard::F7:
		return  key = CreateTitle(L"F7", 25);
	case Keyboard::F8:
		return  key = CreateTitle(L"F8", 25);
	case Keyboard::F9:
		return  key = CreateTitle(L"F9", 25);
	case Keyboard::F10:
		return  key = CreateTitle(L"F10", 25);
	case Keyboard::F11:
		return  key = CreateTitle(L"F11", 25);
	case Keyboard::F12:
		return  key = CreateTitle(L"F12", 25);
	case Keyboard::F13:
		return  key = CreateTitle(L"F13", 25);
	case Keyboard::F14:
		return  key = CreateTitle(L"F14", 25);
	case Keyboard::F15:
		return  key = CreateTitle(L"F15", 25);
	case Keyboard::Pause:
		return  key = CreateTitle(L"Pause", 25);
	case Keyboard::KeyCount:
		return  key = CreateTitle(L"KeyCount", 25);
	default:
		return key = CreateTitle(L"Unknown", 25);
	}
}
Text Options::CreateTitle(const wstring& text, const int size)
{
	Text _Text;
	_Text.setFont(*rec.font);
	_Text.setString(text);
	_Text.setCharacterSize(size);
	_Text.setFillColor(Color::Black);
	return _Text;
}

