#include "FoodServer.h"

#include "IRegistrator.h"

#include "MoveObjectServer.h"

#include "Generator.h"
#include "Constants.h"

constexpr int RESPAWN_TIME = 3000;

namespace Server
{
	Food::Food(IRegistrator* iRegistrator) : Objects(iRegistrator, Vector2f(0.f, 0.f), GameConstants::FOOD_MASS), Respawn(RESPAWN_TIME)
	{
		type = ObjectType::FOOD;
		m_state = ObjectState::Respawnable;
		m_ListColors = getFoodColors();
		m_ColorIndex = genNumber<size_t>(0, m_ListColors.size() - 1);

		localBounds = { _center.x - getRadius(), _center.y - getRadius(), getRadius() * 2.f, getRadius() * 2.f };
	}

	bool Food::checkEaten(Objects& eatingObj)
	{
		if (!active || !eatingObj.isActive())
			return false;
		if (eatingObj.Eating((*this), -getRadius()))
		{
			Respawn::reset();
			registrator->unregisterAuxiliary(shared_from_this());
			active = false;
			return true;
		}
		return false;
	}

	bool Food::Eat(Objects& obj)
	{
		return false; //can't eat;
	}

	void Food::TimeElapsed(int diff)
	{
		if (!active)
		{
			Respawn::update(diff);
			if (Respawn::isReady())
			{
				respawn();
			}
		}
	}

	sf::FloatRect Food::getMBR() const
	{
		return localBounds;
	}

	void Food::respawn()
	{
		if (registrator->spawn(getRadius(), _center))
		{
			registrator->registerAuxiliary(shared_from_this());
			active = true;
		}
	}
	nlohmann::json Food::toStaticJson() const
	{
		return nlohmann::json();
	}
	nlohmann::json Food::toPersistentJson() const
	{
		return nlohmann::json();
	}
}