#include "ThornServer.h"
#include "MoveObjectServer.h"
#include <iostream>

constexpr int RESPAWN_TIME = 3000;

namespace Server
{
	Thorn::Thorn() : Objects(Vector2f(0, 0), _ThornSpriteMass), elapsedRespTime(0)
	{

	}

	void Thorn::TimeElapsed(int diff) 
	{
		if (state == States::READY_TO_RESPAWN)
		{
			update(diff);
		}
	}

	void Thorn::update(int diff)
	{
		elapsedRespTime += diff;
		if (elapsedRespTime >= RESPAWN_TIME)
		{
			elapsedRespTime = 0;
			state = States::READY_TO_LIVE;
		}
	}

	bool Thorn::checkEaten(MoveObject* obj)
	{
		if (!obj->isLive() || !isLive())
			return false;
		if (obj->_mass > _ThornSpriteMass * 1.2f && obj->Eating(*this, -getRadius()))
		{
			obj->readyToWaekend = true;
			setEatenState();
			return true;	
		}
		return false;
	}

	void Thorn::setEatenState()
	{
		state = States::READY_TO_RESPAWN;
	}
	
}