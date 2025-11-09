#include "ThornServer.h"
#include "IRegistrator.h"
#include "MoveObjectServer.h"

#include "Constants.h"
#include <iostream>

constexpr int RESPAWN_TIME = 3000;

namespace Server
{
	Thorn::Thorn(IRegistrator* iRegistrator) : Objects(iRegistrator, Vector2f(0, 0), GameConstants::THORN_MASS), Respawn(RESPAWN_TIME)
	{
		type = ObjectType::THORN;
		m_state = ObjectState::Respawnable;
	}

	bool Thorn::Eat(Objects& obj)
	{
		return false; //can't eat;
	}

	bool Thorn::checkEaten(Objects& eatingObj)
	{
		if (!active || !eatingObj.isActive()) return false;

		if (eatingObj.getMass() > _mass * 1.2f && eatingObj.Eating(*this, -getRadius()))
		{
			/*obj->readyToWaekend = true;*/
			active = false;
			registrator->unregisterAuxiliary(shared_from_this());
			Respawn::reset();
			return true;
		}
		return false;
	}

	/*nlohmann::json Thorn::toJson()
	{
		nlohmann::json res;
		res["type"] = type;
		auto staticJs = toStaticJson();
		if (staticJs != lastSnapshot || staticSentDataTime >= ServerConstants::TIMER_SENT_STATIC_DATA) 
		{
			res["staticData"] = staticJs;
			staticSentDataTime = 0;
		}

		res["persistentData"] = toPersistentJson();
		return res;
	}*/

	void Thorn::TimeElapsed(int diff)
	{
		if (!active)
		{
			Respawn::update(diff);
			if (Respawn::isReady())
			{
				respawn();
			}
		}
		else
		{
			staticSentDataTime += diff;
		}
	}

	nlohmann::json Thorn::toStaticJson() const
	{
		return {
			{"Center", { _center.x, _center.y }},
			{"Radius", getRadius()},
			{"id", getID()},
		};
	}

	nlohmann::json Thorn::toPersistentJson() const
	{
		return nlohmann::json();
	}

	void Thorn::respawn()
	{
		if (registrator->spawn(getRadius(), _center))
		{
			registrator->registerAuxiliary(shared_from_this());
			staticSentDataTime = 0;
			active = true;
		}
	}
	
}