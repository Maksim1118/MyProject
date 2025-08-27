#include "HeroServer.h"
#include "BaseOld.h"

#include <iostream>

namespace Server
{
	Hero::Hero(std::unordered_map <std::string, std::shared_ptr<Feed>>& feeds, const string& name) : Bot(), m_ListFeeds(feeds)
	{
		m_Name = name;
		Feeded = false;
		m_IsGameOver = false;
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
		if (state == States::READY_TO_REMOVE)
		{
			m_IsGameOver = true;
			return;
		}
		Bot::TimeElapsed(diff);
	}

	void Hero::setEatenState()
	{
		state = States::READY_TO_REMOVE;
	}

	bool Hero::isGameOver()
	{
		return m_IsGameOver;
	}

	void Hero::createFeed(Objects& obj)
	{
		shared_ptr<Feed> f = make_shared<Feed>();
		obj._mass -= f->getMass();
		f->setCenter(obj.getCenter());
		Vector2f Dir = getIdentityVector(GetDiff(_Mouse,obj.getCenter()));
		f->setV(Dir * 0.5f);
		f->setParentCenter(obj.getCenter());
		f->setParentRadius(obj.getRadius());
		m_ListFeeds[f->getID()] = f;
	}

	void Hero::setFeeded()
	{
		if (Splitted)
		{
			for (auto& p : pieces)
			{
				if (p.second->getMass() < 400.f)
					continue;
				createFeed(*p.second);
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