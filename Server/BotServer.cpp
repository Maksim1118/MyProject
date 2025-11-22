#include "BotServer.h"

#include "IRegistrator.h"

#include "PiecesList.h"
#include "BaseOld.h"
#include "Constants.h"
#include "Generator.h"

#include <unordered_set>

#include <random>

constexpr int RESPAWN_TIME = 5000;

constexpr float GROWTH_FACTOR = 8.f;
constexpr float kRepulsiveThorn = 4.f;

using namespace std;
using namespace sf;

namespace Server
{

	Bot::Bot(IRegistrator* iRegistrator) : MoveObject(iRegistrator, Vector2f(0, 0), 1000/*GameConstants::MIN_ENTITY_MASS*/, Vector2f(0, 0)), Respawn(RESPAWN_TIME)
		,m_piecesList(make_unique<PiecesList>(this, registrator))
	{
		type = ObjectType::BOT;
		m_state = ObjectState::Respawnable;
		prevMass = _mass;
		Splitted = false;
		colB = (BotsColor)(rand() % 6);
		count = 0;

	}

	void Bot::TimeElapsed(int diff)
	{
		if (active)
		{
			Vector2f target = calcTargetPoint();
			if (Splitted)
			{
				m_piecesList->update(diff, target);

				if (m_piecesList->getSize() == 1)
				{
					auto piece = m_piecesList->getList().begin();
					_mass = piece->second->getMass();
					_center = piece->second->getCenter();
					V = piece->second->getSpeed();
					m_piecesList->remove(piece->first);

					Splitted = false;
					registrator->registerAuxiliary(shared_from_this());
				}
			}
			else
			{
				hungry(diff);

				m_dir = GetCyclicDiff(_center, target, MapConstants::mapWidth, MapConstants::mapHeight);
				float len = GetLen(m_dir);
				float halfR = getRadius() / 2.f;
				
				float kF = GameConstants::FORCE_KOEF;
				float kV = 1.f;

				if (len < halfR)
				{
					kF = len / halfR;
					kV = len / halfR;
				}

				float maxV = 2.0f / sqrt(_mass);
				Vector2f f = calcAttractionForce(getIdentityVector(m_dir), kF);
				Vector2f a = calcAcceleration(f, _mass);
				Vector2f v = calcSpeed(V, a, diff, maxV, kV);

				Move(v);

				registrator->moveObj(shared_from_this());
			}
		}
		else
		{
			Respawn::update(diff);
			if (Respawn::isReady())
			{
				respawn();
			}
		}
		
	}
	void Bot::hungry(int diff)
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

	bool Bot::checkEaten(Objects& eatingObj)
	{
		if (!active|| !eatingObj.isActive()) return false;

		if (eatingObj.getMass() > _mass * 1.2f && eatingObj.Eating(*this, -min(getRadius(), eatingObj.getRadius())))
		{
			registrator->unregisterAuxiliary(shared_from_this());		
			Respawn::reset();
			active = false;
			return true;
		}
		return false;
	}

	void Bot::removePiece(const std::string& id)
	{
		m_piecesList->remove(id);
		if (m_piecesList->getSize() == 0)
		{
			active = false;
		}
	}

	Vector2f Bot::calcTargetPoint() const 
	{
		float viewR = GROWTH_FACTOR * pow(_mass + 1, 0.4f);
		float wRect = max(MapConstants::mapWidth, viewR + viewR);
		Vector2f positionRect = { _center.x - viewR, _center.y - viewR };
		auto& list = registrator->getNearObjects(FloatRect(positionRect,Vector2f(wRect, wRect)));

		Vector2f targetP = _center;
		float bestScore = numeric_limits<float>::min();

		for (const auto& obj: list)
		{
			if (obj->getType() != ObjectType::FOOD && obj->getType() != ObjectType::THORN)
			{
				continue;
			}

			Vector2f tempDir = GetCyclicDiff(_center, obj->getCenter(), MapConstants::mapWidth, MapConstants::mapHeight);
			float dist = GetLen(tempDir);
			if (dist < min(getRadius(), obj->getRadius() * 0.99f))
			{
				continue;
			}

			if (obj->getType() == ObjectType::FOOD)
			{
				float score = 1 / (dist + 1);
				if (score > bestScore)
				{
					bestScore = score;
					targetP = obj->getCenter();
				}
			}

			else if (obj->getType() == ObjectType::THORN)
			{
				float score = -kRepulsiveThorn / (dist + 1);
				if (score > bestScore)
				{
					bestScore = score;
					targetP = _center + (getIdentityVector(tempDir) * -wRect);
				}
			}
		}

		if (GetCyclicDist(targetP, _center, MapConstants::mapWidth, MapConstants::mapHeight) < 0.001f)
		{
			Vector2f Dir = generateDir();
			targetP = _center + Dir * wRect;
		}

		return targetP;
	}

	void Bot::respawn()
	{
		_mass = GameConstants::MIN_ENTITY_MASS;
		V = { 0.f, 0.f };
		_center = { 0.f, 0.f };
		m_dir = { 0.f, 0.f };
		Splitted = false;

		if (registrator->spawn(getRadius(), _center))
		{
			registrator->registerAuxiliary(shared_from_this());
			active = true;
		}
	}

	nlohmann::json Bot::toStaticJson() const
	{
		return nlohmann::json();
	}

	nlohmann::json Bot::toPersistentJson() const
	{
		return nlohmann::json();
	}

	bool Bot::Eat(Objects& obj)
	{
		return obj.checkEaten(*this);
	}

	void Bot::split()
	{
		if (Splitted)
		{
			auto& list = m_piecesList->getList();
			size_t listSize = list.size();
			if (listSize >= GameConstants::MAX_COUNT_PIECES) return;

			vector<shared_ptr<Piece>> tempVec;
			tempVec.reserve(listSize);

			for (auto& piece : list)
			{
				tempVec.push_back(piece.second);
			}

			sort(tempVec.begin(), tempVec.end(), [](const shared_ptr<Piece>& a, const shared_ptr<Piece>& b) {return a->getMass() > b->getMass(); });

			for (auto& p : tempVec)
			{
				if (list.size() == GameConstants::MAX_COUNT_PIECES) break;
				if (p->getMass() < GameConstants::MIN_ENTITY_MASS * 2) break;

				createTwoSplitPiece(p->getMass(), p->getCenter(), p->getSpeed(), getIdentityVector(m_dir));
				m_piecesList->remove(p->getID());
			}
		}
		else
		{
			if (_mass < GameConstants::MIN_ENTITY_MASS * 2) return;
			Splitted = true;
			createTwoSplitPiece(_mass, _center, V, getIdentityVector(m_dir));
			registrator->unregisterAuxiliary(shared_from_this());
			isSplitting = true;
		}
		
	}

	void Bot::createTwoSplitPiece(const float origMass, const sf::Vector2f& origCenter, const sf::Vector2f& origV, sf::Vector2f& dir)
	{	
		if (dir.x == 0.f && dir.y == 0.f)
		{
			dir = generateDir();
		}

		float newMass = origMass / 2.f;

		auto p1 = make_shared<Piece>(registrator, this, newMass, origCenter, origV, PieceState::None);

		Vector2f p2Center = origCenter + dir * getRadius();

		float proj = GameConstants::IMPULSE_FORCE * sqrt(newMass) + origV.x * dir.x + origV.y * dir.y;
		Vector2f p2V0 = proj * dir;

		auto p2 = make_shared<Piece>(registrator, this, newMass, p2Center, p2V0, PieceState::Phantom);
		p2->setTimerSpeed();

		m_piecesList->add(p2->getID(), p2);
		m_piecesList->add(p1->getID(), p1);
	}

	
	void Bot::setPosMouse(float x, float y)
	{
	
		_Mouse = Vector2f(x, y);
	}

	string Bot::getName()
	{
		return m_Name;
	}

	float Bot::getMinRadius() const
	{
		return sqrt(minBotAndPlayerMass);
	}

	Vector2f Bot::getMouse()
	{
		return _Mouse;
	}

}