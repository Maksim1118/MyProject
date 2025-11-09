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

	/*Bot& Bot::operator =(const Bot& other)
	{
		Splitted = other.Splitted;
		V = other.V;
		Timer = other.Timer;
		count = other.count;
		_Mouse = other._Mouse;
		return *this;
	}*/

	/*void Bot::TimeElapsed(int diff)
	{
		hungry(diff);
		if (Splitted == true)
		{
			pieceToSides();
			int i = 0;
			for (auto it = pieces.begin(); it != pieces.end(); )
			{
				if (it->second->readyToWaekend)
				{
					if (setPieceWeakend(it->second))
					{
						it->second->readyToWaekend = false;
						it->second->setEatenState();
						it = pieces.erase(it);
						continue;
					}
				}
				updatePiecePhysic(diff, it->second);
				++i;
				++it;
			}
			together();	
			updateSplitTimer(diff);
		}
		else
		{
			if (readyToWaekend)
			{
				setWeakened();
				readyToWaekend = false;
			}
			if (m_state == States::EATEN)
				update(diff);
			else if (m_state == States::LIVE)
			{
				updateMove(diff);
			}
		}

	}*/


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

				float maxV = /*GameConstants::MAX_V */ 2.0f / sqrt(_mass);
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
		//hungry(diff);
		//if (m_isLive)
		//{
		//	if (Splitted == true)
		//	{
		//		pieceToSides();
		//		int num = 0;
		//		for (auto it = pieces.begin(); it != pieces.end(); ++it)
		//		{
		//			auto& piece = it->second;
		//			/*Vector2f F = calcAttractionForce(piece);*/
		//			Vector2f F(0, 0);
		//			Vector2f dir = getIdentityVector(GetCyclicDiff(piece->getCenter(), _Mouse));
		//			F += calcAttractionForce(dir, 0.7f);
		//			for (auto& piece2 : pieces) 
		//			{
		//				if (piece == piece2.second) {
		//					continue;
		//				}
		//				float lenPiece2 = GetCyclicDist(piece->getCenter(), piece2.second->getCenter());
		//				float minDist = getMinDist(piece->getRadius());
		//				if (lenPiece2 > minDist)
		//				{
		//					Vector2f toPiece21 = getIdentityVector(GetCyclicDiff(piece->getCenter(), piece2.second->getCenter()));
		//					/*float excess = lenPiece2 - minDist;
		//					float strength = pow(excess, 2) * 1e-5f;

		//					F += calcAttractionForce(toPiece21, strength);*/

		//					float strength = /*exp(0.001 * lenPiece2)*/ lenPiece2 * 0.004f;
		//					F += calcAttractionForce(toPiece21, strength);
		//				}
		//			}
		//			float dist = GetCyclicDist(piece->getCenter(), _center);
		//			Vector2f toPiece21 = getIdentityVector(GetCyclicDiff(piece->getCenter(), _center));

		//			float strength = exp(0.003 * dist) * 0.3f;
		//			if (!piece->isExcludedFromMerge())
		//			{
		//				strength *= 2.f;
		//			}
		//			F += calcAttractionForce(toPiece21, strength);

		//			Vector2f a = calcAcceleration(F, piece->getMass());
		//			Vector2f v = calcSpeed(a, diff, piece);
		//			updatePieceMove(piece, v, diff);
		//		
		//			++num;
		//			/*++countPiece;
		

		//		}

		//		mergePieces(diff);

		//		vector<Vector2f> centers;
		//		float massSum = 0.f;
		//		Vector2f speedSum = { 0.f, 0.f };

		//		for (auto& [id, piece] : pieces)
		//		{
		//			centers.push_back(piece->getCenter());
		//			massSum += piece->getMass();
		//			speedSum += piece->getSpeed();
		//		}
		//		this->V = speedSum / (float)pieces.size();
		//		this->_mass = massSum;

		//		Vector2f mainCenter = findCyclicCenter(centers);
		//		setCenter(mainCenter);

		//		if (pieces.size() == 1)
		//		{
		//			/*pieces.begin()->second->setEatenState();*/
		//			pieces.clear();
		//			Splitted = false;
		//		}
		//		
		//	}
		//	else
		//	{
		//		updateMove(diff);
		//	}
		//}
		//else
		//{
		//	update(diff);
		//}

	}
	void Bot::hungry(int diff)
	{
		/*if (isSplitted())
		{
			for (auto& p : pieces)
			{
				if(p.second->_mass > 400/ pieces.size())
					p.second->_mass *= exp(-0.000004f * float(diff));
				else
					p.second->_mass = 400/pieces.size();
			}
		}
		else
		{
			if (_mass > 400)
				_mass *= exp(-0.000004f * float(diff));
			else
				_mass = 400;
		}	*/

		if (_mass > GameConstants::MIN_ENTITY_MASS)
		{
			_mass *= exp(GameConstants::HUNGRY_KOEF * diff);
			if (_mass < GameConstants::MIN_ENTITY_MASS)
			{
				_mass = GameConstants::MIN_ENTITY_MASS;
			}
		}
	}


	//void Bot::udateMass()
	//{
	//	float tmpMass = 0.f;
	//	for (const auto& piece : pieces)
	//	{
	//		if (piece.second->isLive())
	//		{
	//			tmpMass += piece.second->getMass();
	//		}
	//		else
	//		{
	//			tmpMass -= piece.second->getMass();
	//		}
	//		_mass = tmpMass;		
	//	}
	//}

	//bool Bot::createPiece(float& mass, Vector2f& center)
	//{
	//	if (mass < 800.f)
	//		return false;
	//	for (int i = 0; i < 2; i++)
	//	{
	//		shared_ptr<Piece> p = make_shared<Piece>(selfPtr);
	//		registerPiece(pieces, p);

	//		p->_mass = mass / 2;
	//		p->setExcludedFlag(true);
	//		p->setMaxV(0.5f / p->getRadius() * (i * 0.7 + 1));
	//		Vector2f toMouse1 = getIdentityVector(GetCyclicDiff(center, _Mouse));
	//		p->setV(V);
	//		p->setCenter(center + toMouse1 * (float)i);
	//	}
	//	return true;
	//}

	//float Bot::getMinDist(float radius)
	//{
	//	const float k = 10;
	//	float sizeFactor = max(1.f, radius);
	//	return radius * k * (1.f / sizeFactor);
	//}

	//void Bot::updatePieceMove(std::shared_ptr<Piece>& piece, sf::Vector2f& v, int diff)
	//{
	//	piece->setV(v);
	//	piece->Move(v * (float)diff);
	//}

	//bool Bot::canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2)
	//{
	//	float dist = GetCyclicDist(p1->getCenter(), p2->getCenter());
	//	if (dist <= p1->getRadius() || dist <= p2->getRadius())
	//	{
	//		return true;
	//	}
	//	return false;
	//}


	//void Bot::together()
	//{
	//	int count = 0;
	//	Vector2f co(0, 0);
	//	_mass = 0;
	//	V = { 0.f, 0.f };

	//	vector<Vector2f> centers;
	//	for (auto& piece : pieces)
	//	{
	//		V += piece.second->getSpeed();
	//		_mass += piece.second->_mass;
	//		centers.push_back(piece.second->getCenter());
	//	}

	//	co = findCyclicCenter(centers);
	//	setCenter(co);

	//	/*auto it = max_element(pieces.begin(), pieces.end(),
	//		[](auto a, auto b)
	//		{
	//			return b.second->getMass() > a.second->getMass();
	//		});
	//	for (auto& piece : pieces)
	//	{
	//		_mass += piece.second->_mass;
	//		co += GetCyclicDiff(piece.second->getCenter(), it->second->getCenter());
	//		V += piece.second->getSpeed();
	//		count++;
	//	}
	//	co /= (float)count;
	//	co += it->second->getCenter();

	//	V /= (float)count;
	//	setCenter(normalizeCoord(co));	*/
	//}

	//void Bot::mergePieces(int diff)
	//{
	//	vector<Vector2f> centersIncludedPieces;
	//	for (const auto& piece : pieces)
	//	{
	//		if (!piece.second->isExcludedFromMerge())
	//		{
	//			centersIncludedPieces.push_back(piece.second->getCenter());
	//		}
	//	}
	//	Vector2f centerIncludedPieces = findCyclicCenter(centersIncludedPieces);
	//	
	//	bool mergedAny = true;

	//	while (mergedAny)
	//	{
	//		mergedAny = false;

	//		PieceList newPieces;

	//		for (auto it = pieces.begin(); it != pieces.end(); )
	//		{
	//			auto& piece = it->second;
	//			bool merged = false;

	//			for (auto it2 = pieces.begin(); it2 != pieces.end(); )
	//			{
	//				auto& piece2 = it2->second;

	//				if (piece == piece2)
	//				{
	//					++it2;
	//					continue;
	//				}

	//				if (piece->isExcludedFromMerge() || piece2->isExcludedFromMerge())
	//				{
	//					++it2;
	//					continue;
	//				}

	//				Vector2f dir = getIdentityVector(GetCyclicDiff(piece->getCenter(), centerIncludedPieces));
	//				Vector2f F = calcAttractionForce(dir, 0.07f);
	//				Vector2f a = calcAcceleration(F, piece->getMass());
	//				Vector2f v = calcSpeed(a, diff, piece);
	//				updatePieceMove(piece, v, diff);

	//				if (canBeMerged(piece, piece2))
	//				{
	//					auto newPiece = std::make_shared<Piece>(selfPtr);


	//					vector<Vector2f> centers = { piece->getCenter(), piece2->getCenter() };
	//					Vector2f newCenter = findCyclicCenter(centers);

	//					Vector2f newSpeed = (piece->getSpeed() + piece2->getSpeed()) / 2.f;

	//					newPiece->setCenter(newCenter);
	//					newPiece->_mass = piece->getMass() + piece2->getMass();
	//					newPiece->setV(newSpeed);
	//					newPiece->setMaxV(0.6f / newPiece->getRadius());

	//					piece2->setEatenState();
	//					piece->setEatenState();

	//					it2 = pieces.erase(it2);
	//					it = pieces.erase(it);

	//					registerPiece(newPieces, newPiece);


	//					merged = true;
	//					mergedAny = true;
	//					break;
	//				}
	//				else
	//				{
	//					++it2;
	//				}
	//			}

	//			if (!merged)
	//			{
	//				++it;
	//			}
	//		}

	//		for (auto& np : newPieces)
	//		{
	//			pieces[np.second->getID()] = np.second;
	//		}
	//	}
	//}

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


	/*void Bot::removePiece(const string& id)
	{
		m_piecesList->remove(id);
		auto it = pieces.find(id);
		if (it != pieces.end())
		{
			pieces.erase(it);
		}

		if (pieces.empty())
		{
			setEatenState();
			if (Splitted)
			{
				Splitted = false;
			}
		}
	}*/

	bool Bot::Eat(Objects& obj)
	{
		return obj.checkEaten(*this);
	}

	//void Bot::pieceToSides()
	//{
	//	/*for (auto& piece : pieces)
	//	{
	//		Vector2f center = piece.second->getCenter();
	//		for (auto& piece2 : pieces)
	//		{
	//			if (&piece == &piece2)
	//			{
	//				continue;
	//			}
	//			float lenPiece2 = GetCyclicDist(piece.second->getCenter(), piece2.second->getCenter());
	//			float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
	//			if (lenPiece2 < sumRadius)
	//			{
	//				Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
	//				piece2.second->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
	//				piece.second->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
	//			}
	//		}
	//	}*/
	//	for (auto& piece : pieces)
	//	{
	//		Vector2f center = piece.second->getCenter();
	//		for (auto& piece2 : pieces)
	//		{
	//			if (&piece == &piece2)
	//			{
	//				continue;
	//			}

	//			if (piece.second->isExcludedFromMerge() || piece2.second->isExcludedFromMerge())
	//			{
	//				float lenPiece2 = GetCyclicDist(piece.second->getCenter(), piece2.second->getCenter());
	//				float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
	//				if (lenPiece2 < sumRadius)
	//				{
	//					Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
	//					piece2.second->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
	//					piece.second->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
	//				}
	//			}
	//		}
	//	}
	//	
	//}



	//Vector2f Bot::calcAttractionForce(shared_ptr<Piece>& piece)
	//{
	//	Vector2f F(0, 0);
	//	Vector2f center = piece->getCenter();
	//	F += identityVector(GetCyclicDiff(_Mouse,center)) * 0.02f;
	//	for (auto& piece2 : pieces) {
	//		if (piece == piece2.second) {
	//			continue;
	//		}
	//		float lenPiece2 = GetCyclicDist(piece->getCenter(), piece2.second->getCenter());
	//		float minDist = getMinDist(piece->getRadius());
	//		/*if (lenPiece2 > sumRadius)
	//		{
	//			Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
	//			F += toPiece21 * (float)0.01;
	//		}*/
	//		if (lenPiece2 > minDist)
	//		{
	//			Vector2f toPiece21 = identityVector(piece2.second->getCenter() - center);
	//			float excess = lenPiece2 - minDist;
	//			float strength = pow(excess, 2) * 1e-5f;

	//			F += toPiece21 * strength;
	//		}
	//	}
	//	return F;
	//}

	//sf::Vector2f Bot::calcAttractionForce(sf::Vector2f& dir, float koef)
	//{
	//	return dir * koef;
	//}

	//Vector2f Bot::calcAttractionForceToMouse(shared_ptr<Piece>& piece)
	//{
	//	Vector2f dir = GetCyclicDiff(_Mouse, piece->getCenter());
	//	Vector2f force = identityVector(dir);
	//	return force * 0.01f;
	//}

	//Vector2f Bot::calcAcceleration(Vector2f F, float mass)
	//{
	//	Vector2f a = F / mass;
	//	float lenA = GetLen(a);
	//	float maxA = 2.0 / mass;
	//	if (lenA > maxA) {
	//		a = a / lenA * (float)maxA;
	//	}
	//	return a;
	//}

	//Vector2f Bot::calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece)
	//{
	//	Vector2f v{ 0.f,0.f };
	//	v = piece.get()->getSpeed() + a * (float)diff * (float)1.0;
	//	float lenV = GetLen(v);
	//	float maxV = piece.get()->getMaxV();
	//	/*float lenMouse = GetCyclicDist(_Mouse,getCenter());*/
	//	float lenMouse = GetCyclicDist(_Mouse, piece->getCenter());
	//	if (lenMouse < 40.f)
	//	{
	//		maxV *= (lenMouse / 40.f);
	//	}
	//	if (lenV > maxV) {
	//		/*v *= (float)maxV / lenV;*/
	//		v = v / lenV * maxV;
	//	}
	//	return v;
	//}

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
		/*
		if (_mass < 800.f)
			return;
		if (Splitted == true)
		{
			if (pieces.size() >= 8)
			{
				return;
			}
			unordered_map<string, shared_ptr<Piece>> oldPieces = pieces;
			pieces.clear();
			for (auto&p = oldPieces.begin(); p != oldPieces.end();)
			{
				if (pieces.size()+oldPieces.size() >= 8)
				{
					pieces.insert(*p);
					p++;
					continue;
				}
				if (!createPiece(p->second->_mass, p->second->getCenter()))
				{
					pieces.insert(*p);
					
				}
				else
				{
					p->second->setEatenState();
				}
				p = oldPieces.erase(p);
			}
			return;
		}
		Splitted = true;
		createPiece(_mass, getCenter());*/

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

	/*void Bot::setWeakened()
	{
		Splitted = true;
		count = _mass / 400;
		if (count > 8)
			count = 8;
		float remainMass = _mass;

		for (int i = 0; i < count; i++)
		{
			shared_ptr<Piece> p = make_shared<Piece>(selfPtr);
			registerPiece(pieces, p);
			if (i == count - 1)
			{
				p->_mass = remainMass;
			}
			else
			{
				p->_mass = remainMass / (count - i) * (float)(rand() % 40 + 80) / 100.0f;
			}
			remainMass -= p->_mass;
			p->setMaxV(1.0 / p->getRadius());
			p->setCenter(getCenter());
			p->m_ParentColor = static_cast<int>(colB);
		}
	}

	bool Bot::setPieceWeakend(shared_ptr<Piece> &obj)
	{
		if (obj->_mass < 400)
			return false;
		if (pieces.size() >= 8)
			return false;
		count = obj->_mass / 400;
		if (count + pieces.size() > 8)
			count = 8 - pieces.size() + 1;
		if (count == 1)
			return false;
		float remainMass = obj->_mass;
		for (int i = 0; i < count; i++)
		{
			shared_ptr<Piece> p = make_shared<Piece>(selfPtr);
			registerPiece(pieces, p);

			if (i == count - 1)
			{
				p->_mass = remainMass;
			}
			else
			{
				p->_mass = remainMass / (count - i) * (float)(rand() % 40 + 80) / 100.0f;
			}
			remainMass -= p->_mass;
			p->setMaxV(1.0 / p->getRadius());
			p->m_ParentColor = static_cast<int>(colB);
			p->setCenter(obj->getCenter());
		}
		return true;	
	}*/

	//void Bot::updateMove(float diff)
	//{
	//	Vector2f vector = getCenter();
	//	Vector2f distMouse = GetCyclicDiff(vector, _Mouse);
	//	Vector2f d = getIdentityVector(distMouse);
	//	Vector2f a = d * 0.00014f;

	//	V += a * (float)diff;
	//	float lenV = GetLen(V);
	//	float len = GetLen(distMouse);
	//	float maxV = 0.04f;
	//	if (len < getRadius())
	//	{
	//		maxV *= (len / getRadius());
	//	}
	//	if (lenV > maxV)
	//	{
	//		V = V / lenV * maxV;
	//	}
	//	Move(V * diff);
	//}

	//void Bot::reset()
	//{
	//	_mass = minBotAndPlayerMass;
	//	Splitted = false;
	//	colB = (BotsColor)(rand() % 6);
	//	Timer = 50000;
	//	count = 0;
	//	pieces.clear();
	//}

	//void Bot::registerPiece(PieceList& list, shared_ptr<Piece>& piece)
	//{
	//	list[piece->getID()] = piece;
	//	assert(registerFunc);
	//	registerFunc(piece);
	//}

	void Bot::setPosMouse(float x, float y)
	{
	
		_Mouse = Vector2f(x, y);
	}

	//void Bot::setEatenState()
	//{
	//	m_state = States::READY_TO_RESPAWN;
	//}

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

	/*bool Bot::isSplitted()
	{
		return Splitted;
	}*/
}


//namespace Server
//{
//
//	Bot::Bot() :MoveObject(Vector2f(0, 0), minMass), respawnTime(5000), curTime(0)
//	{
//		prevMass = _mass;
//		Splitted = false;
//		colB = (BotsColor)(rand() % 6);
//		Timer = 0;
//		count = 0;
//	}
//
//	Bot& Bot::operator =(const Bot& other)
//	{
//		Splitted = other.Splitted;
//		V = other.V;
//		Timer = other.Timer;
//		count = other.count;
//		_Mouse = other._Mouse;
//		return *this;
//	}
//
//	void Bot::TimeElapsed(int diff)
//	{
//		hungry(diff);
//		if (Splitted == true)
//		{
//			m_state = States::SPLITTED;
//			pieceToSides();
//			for (auto it = pieces.begin(); it != pieces.end(); )
//			{
//				if (it->second->readyToWaekend)
//				{
//					it->second->readyToWaekend = false;
//					if (setPieceWeakend(it->second))
//					{
//						it = pieces.erase(it);
//						continue;
//					}
//				}
//
//				Vector2f F = calcRepulsiveForce(it->second);
//				Vector2f a = calcAcceleration(F, it->second);
//				Vector2f v = calcSpeed(a, diff, it->second);
//				it->second->setV(v);
//				it->second->Move(it->second->getSpeed() * (float)diff);
//
//				++it;
//			}
//			together();
//		}
//		else
//		{
//			if (readyToWaekend)
//			{
//				setWeakened();
//				readyToWaekend = false;
//			}
//			if (m_state == States::EATEN)
//				update(diff);
//			else if (m_state == States::LIVE)
//			{
//				Vector2f vector = getCenter();
//
//				Vector2f d = getIdentityVector(_Mouse - vector);
//				Vector2f a = d * 0.00014f;
//
//				V += a * (float)diff;
//				float lenV = GetLen(V, Vector2f(0, 0));
//				float len = GetLen(_Mouse - vector);
//				float maxV = 0.04f;
//				if (len < getRadius())
//				{
//					maxV *= (len / getRadius());
//				}
//				if (lenV > maxV)
//				{
//					V = V / lenV * maxV;
//				}
//				setCenter(getCenter() + V * (float)diff);
//			}
//		}
//
//		if (Splitted == true)
//		{
//			Timer -= diff;
//			if (Timer < 0)
//			{
//				together();
//				Splitted = false;
//				m_state = States::LIVE;
//			}
//		}
//	}
//
//	void Bot::hungry(int diff)
//	{
//		if (isSplitted())
//		{
//			for (auto& p : pieces)
//			{
//				if (p.second->_mass > 400 / pieces.size())
//					p.second->_mass *= exp(-0.000004f * float(diff));
//				else
//					p.second->_mass = 400 / pieces.size();
//			}
//		}
//		else
//		{
//			if (_mass > 400)
//				_mass *= exp(-0.000004f * float(diff));
//			else
//				_mass = 400;
//		}
//	}
//
//	void Bot::update(int diff)
//	{
//		curTime += diff;
//		if (curTime >= respawnTime)
//		{
//			/*m_state = States::READY_TO_NEXT_ACTION;
//			curTime = 0;
//			_mass = 0;
//			V = { 0.f, 0.f };
//			_center = { 0.f, 0.f };
//			_Mouse = { 0.f, 0.f };*/
//		}
//	}
//
//	void Bot::udateMass()
//	{
//		float tmpMass = 0.f;
//		for (const auto& piece : pieces)
//		{
//			if (piece.second->m_isLive())
//			{
//				tmpMass += piece.second->getMass();
//			}
//			else
//			{
//				tmpMass -= piece.second->getMass();
//			}
//			_mass = tmpMass;
//		}
//	}
//
//	bool Bot::createPiece(float& mass, Vector2f& center)
//	{
//		if (mass < 800.f)
//			return false;
//		Timer = 5000;
//		for (int i = 0; i < 2; i++)
//		{
//			shared_ptr<Piece> p = make_shared<Piece>();
//			pieces[p->getID()] = p;
//
//			p->_mass = mass / 2;
//			p->setMaxV(0.7f / p->getRadius() * (i + 1));
//			Vector2f toMouse1 = getIdentityVector(_Mouse - center);
//			p->setV(toMouse1 * p->getMaxV());
//			p->setCenter(center + toMouse1 * (float)i);
//			p->m_ParentColor = static_cast<int>(colB);
//		}
//		return true;
//	}
//
//	void Bot::together()
//	{
//		int count = 0;
//		Vector2f co(0, 0);
//		_mass = 0;
//		V = { 0.f, 0.f };
//		for (auto& piece : pieces)
//		{
//			_mass += piece.second->_mass;
//			co += piece.second->getCenter();
//			V += piece.second->getSpeed();
//			count++;
//		}
//		co /= (float)count;
//		V /= (float)count;
//		setCenter(co);
//	}
//
//	bool Bot::checkEaten(MoveObject* other)
//	{
//		if (!m_isLive() || !other->m_isLive())
//			return false;
//		if (isSplitted())
//		{
//			for (auto& it = pieces.begin(); it != pieces.end();)
//			{
//				if (it->second->checkEaten(other))
//				{
//					it = pieces.erase(it);
//				}
//				else
//				{
//					++it;
//				}
//
//			}
//
//			if (pieces.empty())
//			{
//				setEatenState();
//				Splitted = false;
//				return true;
//			}
//		}
//		else
//		{
//			if (other->_mass > _mass * 1.2f && other->Eating(*this, -min(getRadius(), other->getRadius())))
//			{
//				setEatenState();
//				return true;
//			}
//		}
//		return false;
//	}
//
//	void Bot::Eat(Objects* obj)
//	{
//		if (isSplitted())
//		{
//			for (auto Piece = pieces.begin(); Piece != pieces.end(); ++Piece)
//			{
//				obj->checkEaten(Piece->second.get());
//			}
//		}
//		else
//		{
//			obj->checkEaten(this);
//		}
//	}
//
//	void Bot::pieceToSides()
//	{
//		for (auto& piece : pieces)
//		{
//			Vector2f center = piece.second->getCenter();
//			for (auto& piece2 : pieces)
//			{
//				if (&piece == &piece2)
//				{
//					continue;
//				}
//				float lenPiece2 = GetLen(piece.second->getCenter(), piece2.second->getCenter());
//				float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
//				if (lenPiece2 < sumRadius)
//				{
//					Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
//					piece2.second->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
//					piece.second->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
//				}
//			}
//		}
//	}
//
//	Vector2f Bot::calcRepulsiveForce(shared_ptr<Piece>& piece)
//	{
//		Vector2f F(0, 0);
//		Vector2f center = piece->getCenter();
//		F += getIdentityVector(_Mouse - center);
//		for (auto& piece2 : pieces) {
//			if (piece == piece2.second) {
//				continue;
//			}
//			float lenPiece2 = GetLen(piece->getCenter(), piece2.second->getCenter());
//			float sumRadius = piece->getRadius() + piece2.second->getRadius();
//			if (lenPiece2 > sumRadius)
//			{
//				Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
//				F += toPiece21 * (float)0.01;
//			}
//		}
//		return F;
//	}
//
//	Vector2f Bot::calcAcceleration(Vector2f F, shared_ptr<Piece>& piece)
//	{
//		Vector2f a = F / piece->_mass;
//		float lenA = GetLen(a);
//		float maxA = 2.0 / piece->_mass;
//		if (lenA > maxA) {
//			a = a / lenA * (float)maxA;
//		}
//		return a;
//	}
//
//	Vector2f Bot::calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece)
//	{
//		Vector2f v{ 0.f,0.f };
//		v = piece.get()->getSpeed() + a * (float)diff * (float)1.0;
//		float lenV = GetLen(v);
//		float maxV = piece.get()->getMaxV();
//		float lenMouse = GetLen(_Mouse - getCenter());
//		if (lenMouse < 40)
//		{
//			maxV *= (lenMouse / 40);
//		}
//		if (lenV > maxV) {
//			v *= (float)maxV / lenV;
//		}
//		return v;
//	}
//
//	void Bot::split()
//	{
//		if (_mass < 800.f)
//			return;
//		if (Splitted == true)
//		{
//			if (pieces.size() >= 8)
//			{
//				return;
//			}
//			unordered_map<string, shared_ptr<Piece>> oldPieces = pieces;
//			pieces.clear();
//			for (auto& p = oldPieces.begin(); p != oldPieces.end();)
//			{
//				if (pieces.size() + oldPieces.size() >= 8)
//				{
//					pieces.insert(*p);
//					p++;
//					continue;
//				}
//				if (!createPiece(p->second->_mass, p->second->getCenter()))
//					pieces.insert(*p);
//				p = oldPieces.erase(p);
//			}
//			return;
//		}
//		Splitted = true;
//		pieces.clear();
//		createPiece(_mass, getCenter());
//	}
//
//	void Bot::setWeakened()
//	{
//		Splitted = true;
//		Timer = 5000;
//		count = _mass / 400;
//		if (count > 8)
//			count = 8;
//		float remainMass = _mass;
//		pieces.clear();
//		for (int i = 0; i < count; i++)
//		{
//			shared_ptr<Piece> p = make_shared<Piece>();
//			pieces[p->getID()] = p;
//			if (i == count - 1)
//			{
//				p->_mass = remainMass;
//			}
//			else
//			{
//				p->_mass = remainMass / (count - i) * (float)(rand() % 40 + 80) / 100.0f;
//			}
//			remainMass -= p->_mass;
//			p->setMaxV(1.0 / p->getRadius());
//			p->setCenter(getCenter());
//			p->m_ParentColor = static_cast<int>(colB);
//		}
//	}
//
//	bool Bot::setPieceWeakend(shared_ptr<Piece>& obj)
//	{
//		if (obj->_mass < 400)
//			return false;
//		if (pieces.size() >= 8)
//			return false;
//		count = obj->_mass / 400;
//		if (count + pieces.size() > 8)
//			count = 8 - pieces.size() + 1;
//		if (count == 1)
//			return false;
//		float remainMass = obj->_mass;
//		for (int i = 0; i < count; i++)
//		{
//			shared_ptr<Piece> p = make_shared<Piece>();
//			pieces[p->getID()] = p;
//
//			if (i == count - 1)
//			{
//				p->_mass = remainMass;
//			}
//			else
//			{
//				p->_mass = remainMass / (count - i) * (float)(rand() % 40 + 80) / 100.0f;
//			}
//			remainMass -= p->_mass;
//			p->setMaxV(1.0 / p->getRadius());
//
//			p->setCenter(obj->getCenter());
//		}
//		return true;
//	}
//
//	void Bot::setPosMouse(float x, float y)
//	{
//		_Mouse = Vector2f(x, y);
//	}
//
//	void Bot::setEatenState()
//	{
//		m_state = States::READY_TO_RESPAWN;
//	}
//
//	string Bot::getName()
//	{
//		return name;
//	}
//
//	float Bot::getMinRadius() const
//	{
//		return sqrt(minMass);
//	}
//
//	Vector2f Bot::getMouse()
//	{
//		return _Mouse;
//	}
//
//	bool Bot::isSplitted()
//	{
//		return Splitted;
//	}
//}