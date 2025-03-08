#include "ThornServer.h"
#include "MoveObjectServer.h"
#include <iostream>

namespace Server
{
	Thorn::Thorn() : Objects(Vector2f(0, 0), _ThornSpriteMass), respawnTime(10000), curTime(0)
	{

	}

	void Thorn::TimeElapsed(int diff) 
	{
		if (state == States::EATEN)
		{
			update(diff);
		}
	}

	void Thorn::update(int diff)
	{
		curTime += diff;
		if (curTime >= respawnTime)
		{
			state = States::READY_TO_LIVE;
			curTime = 0;
		}
	}

	bool Thorn::checkEaten(MoveObject* player)
	{
		if (player->state == States::EATEN || state == States::EATEN)
			return false;
		if (player->_mass > _ThornSpriteMass * 1.2f && player->Eating(*this, -getRadius()))
		{
			player->readyToWaekend = true;
			return true;	
		}
		return false;
	}
	
}