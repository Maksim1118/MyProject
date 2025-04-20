#include "HeroServer.h"
#include "BaseOld.h"

#include <iostream>

namespace Server
{
	Hero::Hero(string  text) : Bot(text)
	{
		Feeded = false;
	}

	Hero& Hero::operator =(const Hero& other)
	{
		Splitted = other.Splitted;
		Feeded = other.Feeded;
		pieces = other.pieces;
		listFeeds = other.listFeeds;
		_Mouse = other._Mouse;
		V = other.V;		
		Timer = other.Timer;
		count = other.count;
		return *this;
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
		listFeeds.push_back(f);
	}

	list<shared_ptr<Feed>>& Hero::getListFeeds()
	{
		return listFeeds;
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