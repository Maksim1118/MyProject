#include "HeroServer.h"

#include "PiecesList.h"
#include "Constants.h"
#include "IRegistrator.h"
#include "BaseOld.h"

#include <iostream>

constexpr float MIN_VIEW_RADIUS = 120.f;

namespace Server
{
	Hero::Hero(IRegistrator* iRegistrator,/* std::unordered_map <std::string, std::shared_ptr<Feed>>& feeds,*/ const string& name) : Bot(iRegistrator)/*, listFeeds(feeds)*/
	{
		type = ObjectType::PLAYER;
		m_state = ObjectState::Delitable;
		m_Name = name;
		Feeded = false;
		m_isGameOver = false;
	}

	bool Hero::checkEaten(Objects& eatingObj)
	{
		if (!active || !eatingObj.isActive()) return false;

		if (eatingObj.getMass() > _mass * 1.2f && eatingObj.Eating(*this, -min(getRadius(), eatingObj.getRadius())))
		{
			active = false;
			registrator->unregisterAuxiliary(shared_from_this());
			return true;
		}
		return false;
	}

	void Hero::TimeElapsed(int diff)
	{
		if (!active) return;

		if (Splitted)
		{
			m_piecesList->update(diff, _Mouse);

			if (m_piecesList->getSize() == 1)
			{
				auto piece = m_piecesList->getList().begin();
				_mass = piece->second->getMass();
				_center = piece->second->getCenter();
				V = piece->second->getSpeed();
				m_piecesList->remove(piece->first);

				Splitted = false;
				registrator->registerAuxiliary(shared_from_this());
				isSplitting = false;
			}
		}
		else
		{
			hungry(diff);

			m_dir = GetCyclicDiff(_center, _Mouse, MapConstants::mapWidth, MapConstants::mapHeight);
			float lenDir = hypot(m_dir.x, m_dir.y);
			float radius = getRadius();

			maxV = GameConstants::MAX_V / sqrt(_mass);

			if (lenDir < radius)
			{
				maxV *= (lenDir / radius);
			}
		
			float kV = 1.f;

			Vector2f f = calcAttractionForce(getIdentityVector(m_dir), GameConstants::FORCE_KOEF);
			Vector2f a = calcAcceleration(f, _mass);
			V = calcSpeed(V, a, diff, maxV, kV);

			Move(V);

			registrator->moveObj(shared_from_this());
		}
	}

	sf::FloatRect Hero::getViewArea() const
	{
		float scaleFactor = sqrt(_mass / GameConstants::MIN_ENTITY_MASS);
		float halfSize = MIN_VIEW_RADIUS * scaleFactor;

		float maxR = min(MapConstants::mapWidth / 2.f * 0.95f, MapConstants::mapHeight / 2.f * 0.95f);
		if (halfSize > maxR)
		{
			halfSize = maxR;
		}
		return { _center.x - halfSize, _center.y - halfSize, halfSize * 2.f, halfSize * 2.f };
	}

	bool Hero::isGameOver()
	{
		return m_isGameOver;
	}

	nlohmann::json Hero::toStaticJson() const
	{
		return nlohmann::json();
	}

	nlohmann::json Hero::toPersistentJson() const
	{
		return nlohmann::json();
	}

	bool Hero::isFeeded()
	{
		return Feeded;
	}
}