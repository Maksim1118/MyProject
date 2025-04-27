#include "HeroServer.h"
#include "BaseOld.h"

#include <iostream>

namespace Server
{
	Hero::Hero(list<shared_ptr<Feed>>& feeds, string  text) : m_ListFeeds(feeds), Bot(text)
	{
		Feeded = false;
	}

	Hero& Hero::operator =(const Hero& other)
	{
		Splitted = other.Splitted;
		Feeded = other.Feeded;
		pieces = other.pieces;
		m_ListFeeds = other.m_ListFeeds;
		_Mouse = other._Mouse;
		V = other.V;		
		Timer = other.Timer;
		count = other.count;
		return *this;
	}

	void Hero::TimeElapsed(int diff)
	{
		if (state == States::EATEN)
			return;
		Bot::TimeElapsed(diff);
	}

	void Hero::createFeed(Objects& obj)
	{
		shared_ptr<Feed> f = make_shared<Feed>();
		obj._mass -= f->getMass();
		f->setCenter(obj.getCenter());
		Vector2f Dir = getIdentityVector(_Mouse - obj.getCenter());
		f->setV(Dir * 0.5f);
		f->setParentCenter(obj.getCenter());
		f->setParentRadius(obj.getRadius());
		m_ListFeeds.push_back(f);
	}

	void Hero::setFeeded()
	{
		if (Splitted)
		{
			for (auto& p : pieces)
			{
				if (p->getMass() < 400.f)
					continue;
				createFeed(*p);
			}
		}
		else
		{
			if (_mass < 400.f)
				return;
			createFeed(*this);
		}
	}

	bool Hero::isFeeded()
	{
		return Feeded;
	}
}