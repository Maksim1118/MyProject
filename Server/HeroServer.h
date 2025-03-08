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
		Hero(list<shared_ptr<Food>>& f, string  text);
		Hero& operator =(const Hero&);
		void setFeeded();
		bool isFeeded();
		list<shared_ptr<Food>>& feeds;
	private:
		bool Feeded;
		void createFeed(Objects& obj);
	};
}


