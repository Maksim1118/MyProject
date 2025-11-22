#include "Options.h"

using namespace sf;
using namespace std;


Options& Options::getInstance()
{
	static Options options;
	return options;
}

Options::Options()
	:m_settingConfig(getDefaultConfig()), m_workingConfig(m_settingConfig)
{
    
}

SettingConfig& Options::getDefaultConfig() const
{
	return SettingConfig(50, false, static_cast<int>('e'), static_cast<int>(' '), listResolutions[0].first );
}

bool Options::isResolutionAvailable(const Resolution& resolution) const
{
    return any_of(listResolutions.begin(), listResolutions.end(),
        [&resolution](const auto& pair) { return pair.first == resolution; });
}


const SettingConfig& Options::getSettingConfig() const
{
	return m_settingConfig;
}

void Options::setFullScreen(bool isFullScreen)
{
	m_workingConfig.m_fullScreen = isFullScreen;
}

void Options::setVolume(int volume)
{
	m_workingConfig.m_settingVolume = volume;
}

void Options::setKeyCode(const KeyType type, const int keyCode)
{
    switch (type)
    {
    case KeyType::FEED:
        m_workingConfig.m_key_Feed = keyCode;
        break;

    case KeyType::SPLIT:
        m_workingConfig.m_key_Split = keyCode;
        break;

    default:
        [[fallthrough]];
    }
}

void Options::setResolution(const Resolution& resolution)
{
    if (isResolutionAvailable(resolution))
    {
        m_workingConfig.m_resolution = resolution;
    }
}

void Options::setResolution(const std::string& resolution)
{
    auto it = std::find_if(listResolutions.begin(), listResolutions.end(),
        [&resolution](const auto& pair) { return pair.second == resolution; });

    if (it != listResolutions.end()) 
    {
        setResolution(it->first);
    }
}

void Options::applySetting()
{
    if (m_workingConfig == m_settingConfig)
    {
        return;
    }
    m_settingConfig = m_workingConfig;
}

bool Options::hasChanged()
{
    return !(m_settingConfig == m_workingConfig);
}

