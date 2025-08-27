#include "FoodServer.h"
#include "MoveObjectServer.h"

constexpr int RESPAWN_TIME = 3000;

namespace Server
{
	Food::Food() : Objects(Vector2f(0.f, 0.f), _FoodMass), respawnTime(10000), elapsedRespTime(0), m_isRespawnState(false)
	{
		m_ListColors = getFoodColors();
		m_ColorIndex = rand() % (m_ListColors.size() - 1);
	}

	void Food::TimeElapsed(int diff)
	{
		if (state == States::READY_TO_RESPAWN)
		{
			update(diff);
		}
	}

	void Food::update(int diff)
	{
		elapsedRespTime += diff;
		if (elapsedRespTime >= RESPAWN_TIME)
		{
			state = States::READY_TO_LIVE;
			elapsedRespTime = 0;
		}
	}

	bool Food::checkEaten(MoveObject* obj)
	{
		if (!isLive() || !obj->isLive())
			return false;
		if (obj->Eating((*this), -getRadius()))
		{
			setEatenState();
			return true;
		}
		return false;
	}
	void Food::setEatenState()
	{
		state = States::READY_TO_RESPAWN;
	}
}