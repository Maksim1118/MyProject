#include "HeroServer.h"
#include "BaseOld.h"

#include <iostream>

namespace Server
{
	Hero::Hero(list<shared_ptr<Food>>& f, string  text) : Objects(Vector2f(0, 0), 400.000f),Bot(text), feeds(f)
	{
		Feeded = false;
	}

	Hero& Hero::operator =(const Hero& other)
	{
		Splitted = other.Splitted;
		Feeded = other.Feeded;
		pieces = other.pieces;
		feeds = other.feeds;
		_Mouse = other._Mouse;
		V = other.V;		
		Timer = other.Timer;
		count = other.count;
		return *this;
	}

	void Hero::createFeed(Objects& obj)
	{
		Feed* f = new Feed();
		obj._mass -= f->getMass();
		f->setCenter(obj.getCenter());
		Vector2f Dir = getIdentityVector(_Mouse - obj.getCenter());
		f->setV(Dir * 0.5f);
		f->setParentCenter(getCenter());
		f->setParentRadius(getRadius());
		feeds.push_back(shared_ptr<Food>(f));
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