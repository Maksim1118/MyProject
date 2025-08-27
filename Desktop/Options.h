//#pragma once
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <fstream>
//
//using namespace sf;
//using namespace std;
//
//class Options
//{
//public:
//	Options();
//	bool PosSlider(const float& x, const float& y);
//	bool PosBox(const float& x, const float&  y);
//	bool PosKey(const Text& text, const float& x, const float& y);
//	bool PosButtonClose(const float& x, const float& y);
//	bool PosButtonSave(const float& x, const float& y);
//	void Saved();
//	static void OpenSaved(const char* file);
//	void CreateMus();
//	Text  CreateTitle(const wstring& text, const int size);
//	RectangleShape  CreateLineMus();
//	RectangleShape  CreateRegulator();
//	void moveSlider(int& mouseX);
//	RectangleShape CreateCheckBox();
//	RectangleShape CreateCheckMarkLine1();
//	RectangleShape CreateCheckMarkLine2();
//	Text handleKeyPressed(int& code);	
//	static int SplitedKeyCode;
//	static int FeededKeyCode;
//	static float volume;
//	static bool _isBoxPressed;
//	static bool _isSaveActive;
//	static bool _isKeySpliteChanged;
//	static bool _isKeyFeedingChanged;
//	static bool _IsButtonClosePressed;
//	static float _WindowWidth;
//	static float _WindowHeight;
//protected:
//	bool _isSliderPressed = false;
//
//	RectangleShape BackGround;
//	RectangleShape	_LineMus;
//	RectangleShape _Regulator;
//	RectangleShape _Box;
//	RectangleShape _CheckMark1;
//	RectangleShape _CheckMark2;
//
//	Text Setting;
//	Text Setting_1;
//	Text Setting_2;
//	Text Setting_3;
//	Text Setting_4;
//	Text Save;
//	float tempvol{ 0 };
//    bool tempbool = false;
//	Text FeedingKeyCode;
//	Text SpliteKeyCode;
//	Sprite Close;
//	Sprite SP;
//};

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <string>

#include "SettingConfig.h"


//class SettingConfig
//{
//public:
//	SettingConfig(const int volume, const bool isFullScreen, const char keyFeed, const char keySplit);
//	int getVolume()const;
//	void setVolume(const int volume);
//	void setFullScreen(const bool fullScreen);
//	bool isFullScreen() const;
//	void setKeyCode(const keyBindType keyType, const int keyCode);
//	int getCodeKeyFeed() const;
//	int getCodeKeySplit() const;
//private:
//	int m_settingVolume;
//	bool m_fullScreen;
//	char m_key_Feed;
//	char m_key_Split;
//};


struct Resolution
{
	unsigned int width;
	unsigned int height;
	Resolution(unsigned int width = 1920, unsigned int height = 1080)
		: width(width), height(height) {}

	bool operator==(const Resolution&) const;
};

const std::vector<std::pair<Resolution, std::string>> listResolutions =
{
		{ {1920, 1080}, "1920x1080" }, // Full HD
		{ {2560, 1440}, "2560x1440" }, // QHD
		{ {3840, 2160}, "3840x2160" }, // 4K UHD
		{ {1280, 720}, "1280x720" },   // HD
		{ {800, 600}, "800x600" },     // SVGA
		{ {1024, 768}, "1024x768" },   // XGA
		{ {1600, 900}, "1600x900" },   // HD+
		{ {1366, 768}, "1366x768" },   // HD (WXGA)
		{ {1440, 900}, "1440x900" },   // WXGA+
		{ {1280, 800}, "1280x800" },   // WXGA
		{ {1680, 1050}, "1680x1050" }, // WSXGA+
		{ {2560, 1080}, "2560x1080" }, // UltraWide Full HD
		{ {3440, 1440}, "3440x1440" }, // UltraWide QHD
		{ {5120, 1440}, "5120x1440" }, // Dual QHD
		{ {3840, 1600}, "3840x1600" }, // 4K WQXGA
		{ {1920, 1200}, "1920x1200" }, // WUXGA
		{ {1024, 600}, "1024x600" },   // WSVGA
		{ {640, 480}, "640x480" }      // VGA
};

enum class KeyType
{
	FEED,
	SPLIT
};

struct SettingConfig
{
public:
	SettingConfig(
		const int volume, 
		const bool isFullScreen, 
		const int keyFeed, 
		const int keySplit, 
		const Resolution& resolution);

	bool operator==(const SettingConfig&) const;

	int m_settingVolume;
	bool m_fullScreen;
	int m_key_Feed;
	int m_key_Split;
	Resolution m_resolution;
};


inline SettingConfig::SettingConfig(const int volume, const bool isFullScreen, const int keyFeed, const int keySplit, const Resolution& resolution)
	:m_settingVolume(volume), m_fullScreen(isFullScreen), m_key_Feed(keyFeed), m_key_Split(keySplit), m_resolution(resolution)
{
}

inline bool SettingConfig::operator==(const SettingConfig& other) const
{
	return m_settingVolume == other.m_settingVolume
		&& m_fullScreen == other.m_fullScreen
		&& m_key_Feed == other.m_key_Feed
		&& m_key_Split == other.m_key_Split
		&& m_resolution == other.m_resolution;
}

inline bool Resolution::operator==(const Resolution& other) const
{
	return width == other.width && height == other.height;
}



class Options
{
public:
	static Options& getInstance();
	Options(const Options&) = delete;
	Options& operator =(const Options&) = delete;
	const SettingConfig& getSettingConfig() const;
	void setFullScreen(bool isFullScreen);
	void setVolume(int volume);
	void setKeyCode(const KeyType type, const int keyCode);
	void setResolution(const Resolution& resolution);
	void setResolution(const std::string& resolution);
	void applySetting();
	bool hasChanged();
private:
	Options();
	SettingConfig& getDefaultConfig() const;
	bool isResolutionAvailable(const Resolution& resolution) const;
private:
	SettingConfig m_settingConfig;
	SettingConfig m_workingConfig;
};

//inline int SettingConfig::getVolume()const
//{
//	return m_settingVolume;
//}
//
//inline void SettingConfig::setVolume(const int volume)
//{
//	m_settingVolume = volume;
//}
//
//inline void SettingConfig::setFullScreen(const bool fullScreen)
//{
//	m_fullScreen = fullScreen;
//}
//
//inline bool SettingConfig::isFullScreen() const
//{
//	return m_fullScreen;
//}
//
//inline int SettingConfig::getCodeKeyFeed() const
//{
//	return static_cast<int>(m_key_Feed);
//}
//
//inline int SettingConfig::getCodeKeySplit() const
//{
//	return static_cast<int>(m_key_Split);
//}
//
