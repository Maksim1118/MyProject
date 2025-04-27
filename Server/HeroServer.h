#pragma once
#include "ObjectsServer.h"
#include "BotServer.h"
#include "FeedServer.h"

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
		Hero(list<shared_ptr<Feed>>& feeds, string text);
		Hero& operator =(const Hero&);
		void setFeeded();
		bool isFeeded();
		void TimeElapsed(int diff)override;
	private:
		list<shared_ptr<Feed>> &m_ListFeeds;
		bool Feeded;
		void createFeed(Objects& obj);
	};
}


