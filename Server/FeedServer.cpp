#include "FeedServer.h"

#include "IRegistrator.h"

#include "Constants.h"

namespace Server
{
	Feed::Feed(IRegistrator* iRegistrator) : MoveObject(iRegistrator, Vector2f(0, 0), GameConstants::FEED_MASS, Vector2f(0, 0)), parentCenter( 0.f,0.f ), parentRadius(0.f), m_IsUnderHero(true)
	{
		type = ObjectType::FEED;
		m_state = ObjectState::Delitable;
		colorNum = rand() % 6;
	}

	bool Feed::Eat(Objects& obj)
	{
		return false; //can't eat;
	}

	bool Feed::checkEaten(Objects& eatingObj)
	{
		if (!active || !eatingObj.isActive() || m_IsUnderHero)
			return false;
		if (eatingObj.Eating((*this), -getRadius()))
		{
			active = false;
			registrator->unregisterAuxiliary(shared_from_this());
			return true;
		}
		return false;
	}

	sf::FloatRect Feed::getMBR() const
	{
		return { _center.x - getRadius(), _center.y - getRadius(), getRadius() * 2.f, getRadius() * 2.f };
	}

	void Feed::TimeElapsed(int diff)
	{
		if (GetCyclicDist(parentCenter, getCenter(), MapConstants::mapWidth, MapConstants::mapHeight) > parentRadius * 1.2f)
		{
			if (m_IsUnderHero)
			{
				m_IsUnderHero = false;
			}
			float lenV = GetLen(V);
			float newlenV = lenV - 0.0005f * (float)diff;
			if (newlenV <= 0)
			{
				V = { 0,0 };
			}
			else
			{
				V *= newlenV / lenV;

			}
		}
		Move(V * (float)diff);
	}

	void Feed::setV(Vector2f& newV)
	{
		V = newV;
	}

	void Feed::setParentCenter(Vector2f& newCenter)
	{
		parentCenter = newCenter;
	}

	void Feed::setParentRadius(float newRadius)
	{
		parentRadius = newRadius;
	}

	nlohmann::json Feed::toStaticJson() const
	{
		return nlohmann::json();
	}
	nlohmann::json Feed::toPersistentJson() const
	{
		return nlohmann::json();
	}
}
