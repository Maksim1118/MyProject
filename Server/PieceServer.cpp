#include "PieceServer.h"

#include "IRegistrator.h"
#include "BotServer.h"
#include "Constants.h"
#include "BaseFunctions.h"


#include <iostream>


namespace Server
{
	Piece::Piece(IRegistrator* iRegistrator, Bot* bot, float mass, const sf::Vector2f& center, const sf::Vector2f& v, PieceState ability)
		:
		MoveObject(iRegistrator, center, mass, v)
		, m_parent(bot)
		, ability(ability)
		, target(0, 0)
		, timerActiveSpeed(0)
	{
		type = ObjectType::PIECE;
		m_state = ObjectState::Delitable;
		m_ParentColor = static_cast<int>(m_parent->colB);
		
		initTimerAbility();
	}

	bool Piece::checkEaten(Objects& eatingObj)
	{
		if (!active || !eatingObj.isActive() || (eatingObj.getID() == m_parent->getID())) return false;

		if (eatingObj.getMass() > _mass * 1.2f && eatingObj.Eating(*this, -min(getRadius(), eatingObj.getRadius())))
		{
			active = false;
			registrator->unregisterAuxiliary(shared_from_this());
			m_parent->removePiece(getID());
			return true;
		}
		return false;
	}

	bool Piece::Eat(Objects& obj)
	{
		return obj.checkEaten(*this);
	}


	void Piece::TimeElapsed(int diff)
	{	
		hungry(diff);

		timerActiveSpeed -= diff;
		if (timerActiveSpeed <= 0)
		{
			timerActiveAbility = 0;
		}

		m_dir = GetCyclicDiff(_center, target, MapConstants::mapWidth, MapConstants::mapHeight);
		float lenDir = hypot(m_dir.x, m_dir.y);

		float radius = getRadius();

		float targetMaxV = GameConstants::MAX_V / sqrt(_mass);

		if (lenDir < radius)
		{
			targetMaxV *= (lenDir / radius);
		}

		float kV = 1.f;

		float phantomMaxV = targetMaxV * 2.4f;

		if (timerActiveSpeed > 0)
		{
			float lerpFactor = (float)timerActiveSpeed / TIMER_SPEED;
			maxV = lerpFunc(phantomMaxV, targetMaxV, 1.f - lerpFactor);
		}
		else
		{
			maxV = targetMaxV;
		}


		Vector2f F = calcAttractionForce(getIdentityVector(m_dir), GameConstants::FORCE_KOEF);
		Vector2f a = calcAcceleration(F, _mass);
		V = calcSpeed(V, a, diff, maxV, kV);

		Move(V);

		registrator->moveObj(shared_from_this());

		updateAbility(diff);
	}

	sf::FloatRect Piece::getMBR() const
	{
		return { _center.x - getRadius(), _center.y - getRadius(), getRadius() * 2.f, getRadius() * 2.f };
	}

	void Piece::setTarget(const sf::Vector2f& targetP)
	{
		target = targetP;
	}

	void Piece::hungry(int diff)
	{
		if (_mass > GameConstants::MIN_ENTITY_MASS)
		{
			_mass *= exp(GameConstants::HUNGRY_KOEF * diff);
			if (_mass < GameConstants::MIN_ENTITY_MASS)
			{
				_mass = GameConstants::MIN_ENTITY_MASS;
			}
		}
	}

	PieceState Piece::getAbility() const
	{
		return ability;
	}

	const std::string& Piece::getParentID() const
	{
		return m_parent->getID();
	}

	void Piece::setTimerSpeed()
	{
		timerActiveSpeed = TIMER_SPEED;
	}

	nlohmann::json Piece::toStaticJson() const
	{
		return nlohmann::json();
	}

	nlohmann::json Piece::toPersistentJson() const
	{
		return nlohmann::json();
	}

	void Piece::initTimerAbility()
	{
		switch (ability)
		{
		case PieceState::None:
			timerActiveAbility = EXCLUSION_MERGE_DUR;
			break;
		case PieceState::Merge:
			timerActiveAbility = 0;
			break;
		case PieceState::Phantom:
			timerActiveAbility = PHANTOM_DUR;
			break;
		default:
			break;
		}
	}

	void Piece::updateAbility(int diff)
	{
		if (ability == PieceState::Merge) return;

		timerActiveAbility -= diff;

		if (timerActiveAbility <= 0)
		{
			switch (ability)
			{
			case PieceState::None:
				ability = PieceState::Merge;
				initTimerAbility();
				break;
			case PieceState::Phantom:
				ability = PieceState::None;
				initTimerAbility();
				break;
			default:
				break;
			}
		}
	}
}
