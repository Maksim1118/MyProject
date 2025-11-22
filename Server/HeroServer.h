#pragma once
#include "ObjectsServer.h"
#include "BotServer.h"
#include "FeedServer.h"
#include "Camera.h"
#include "MapSegment.h"

#include <list>
#include <iostream>
#include <memory>
using namespace sf;
using namespace std;

constexpr float K = 0.2f;

namespace Server
{
	class Hero : public Bot
	{
	public:
		Hero(IRegistrator* iRegistrator,/* std::unordered_map<std::string, std::shared_ptr<Feed>>& feeds, */const std::string& name);
		bool checkEaten(Objects& eatingObj) override;
		bool isFeeded();
		void TimeElapsed(int diff)override;
		sf::FloatRect getViewArea() const;

		bool isGameOver();
	private:
		bool Feeded;
		bool m_isGameOver;
	private:
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
	};
}


