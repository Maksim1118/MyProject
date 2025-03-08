#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>

using namespace sf;
using namespace std;

class Options
{
public:
	Options();
	bool PosSlider(const float& x, const float& y);
	bool PosBox(const float& x, const float&  y);
	bool PosKey(const Text& text, const float& x, const float& y);
	bool PosButtonClose(const float& x, const float& y);
	bool PosButtonSave(const float& x, const float& y);
	void Saved();
	static void OpenSaved(const char* file);
	void CreateMus();
	Text  CreateTitle(const wstring& text, const int size);
	RectangleShape  CreateLineMus();
	RectangleShape  CreateRegulator();
	void moveSlider(int& mouseX);
	RectangleShape CreateCheckBox();
	RectangleShape CreateCheckMarkLine1();
	RectangleShape CreateCheckMarkLine2();
	Text handleKeyPressed(int& code);	
	static int SplitedKeyCode;
	static int FeededKeyCode;
	static float volume;
	static bool _isBoxPressed;
	static bool _isSaveActive;
	static bool _isKeySpliteChanged;
	static bool _isKeyFeedingChanged;
	static bool _IsButtonClosePressed;
	static float _WindowWidth;
	static float _WindowHeight;
protected:
	bool _isSliderPressed = false;

	RectangleShape BackGround;
	RectangleShape	_LineMus;
	RectangleShape _Regulator;
	RectangleShape _Box;
	RectangleShape _CheckMark1;
	RectangleShape _CheckMark2;

	Text Setting;
	Text Setting_1;
	Text Setting_2;
	Text Setting_3;
	Text Setting_4;
	Text Save;
	float tempvol{ 0 };
    bool tempbool = false;
	Text FeedingKeyCode;
	Text SpliteKeyCode;
	Sprite Close;
	Sprite SP;
};

