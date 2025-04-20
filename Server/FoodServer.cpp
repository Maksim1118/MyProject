#include "FoodServer.h"
#include "MoveObjectServer.h"

namespace Server
{
	Food::Food() : Objects(Vector2f(0.f, 0.f), _FoodMass), respawnTime(10000), curTime(0)
	{
		colorNum = rand() % 6;
	}

	void Food::TimeElapsed(int diff)
	{
		if (state == States::EATEN)
		{
			update(diff);
		}
	}

	void Food::update(int diff)
	{
		curTime += diff;
		if (curTime >= respawnTime)
		{
			state = States::READY_TO_LIVE;
			curTime = 0;
		}
	}

	bool Food::checkEaten(MoveObject* obj)
	{
		if (state == States::EATEN || obj->state == States::EATEN)
			return false;
		if (obj->Eating((*this), -getRadius()))
		{
			return true;
		}
		return false;
	}
}