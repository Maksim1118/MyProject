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
		Hero(string  text);
		Hero& operator =(const Hero&);
		void setFeeded();
		bool isFeeded();
		list<shared_ptr<Feed>>& getListFeeds();
	private:
		bool Feeded;
		list<shared_ptr<Feed>> listFeeds{};
		void createFeed(Objects& obj);
	};
}


