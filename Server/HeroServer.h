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
		Hero(std::unordered_map<std::string, std::shared_ptr<Feed>>& feeds, const std::string& name);
		Hero& operator =(const Hero&);
		void setFeeded();
		bool isFeeded();
		void TimeElapsed(int diff)override;
		void setEatenState() override;
		bool isGameOver();
	private:
		std::unordered_map <std::string, std::shared_ptr<Feed>>&m_ListFeeds;
		bool Feeded;
		bool m_IsGameOver;
	private:
		void createFeed(Objects& obj);
	};
}


