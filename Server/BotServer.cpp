#include "BotServer.h"

#include "BaseOld.h"

#include <random>

constexpr int RESPAWN_TIME = 5000;

namespace Server
{

	Bot::Bot() :MoveObject(Vector2f(0, 0), minBotAndPlayerMass),elapsedRespTime(0), curTime(0)
	{
		prevMass = _mass;
		Splitted = false;
		colB = (BotsColor)(rand() % 6);
		Timer = 50000;
		count = 0;

	}

	Bot& Bot::operator =(const Bot& other)
	{
		Splitted = other.Splitted;
		V = other.V;
		Timer = other.Timer;
		count = other.count;
		_Mouse = other._Mouse;
		return *this;
	}

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
				cout << "speed" << i << "x: " << it->second->getSpeed().x << endl;
				cout << "speed" << i << "y: " << it->second->getSpeed().y << endl;
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
			if (state == States::EATEN)
				update(diff);
			else if (state == States::LIVE)
			{
				updateMove(diff);
			}
		}

	}*/


	void Bot::TimeElapsed(int diff)
	{
		hungry(diff);
		if (Splitted == true)
		{
			pieceToSides();
			int num = 0;
			for (auto it = pieces.begin(); it != pieces.end(); ++it)
			{
				auto& piece = it->second;
				/*Vector2f F = calcAttractionForce(piece);*/
				Vector2f F(0, 0);
				Vector2f dir = getIdentityVector(GetDiff(piece->getCenter(), _Mouse));
				F += calcAttractionForce(dir, 0.7f);
				//for (auto& piece2 : pieces) 
				//{
				//	if (piece == piece2.second) {
				//		continue;
				//	}
				//	float lenPiece2 = GetDist(piece->getCenter(), piece2.second->getCenter());
				//	float minDist = getMinDist(piece->getRadius());
				//	if (lenPiece2 > minDist)
				//	{
				//		Vector2f toPiece21 = getIdentityVector(GetDiff(piece->getCenter(), piece2.second->getCenter()));
				//		/*float excess = lenPiece2 - minDist;
				//		float strength = pow(excess, 2) * 1e-5f;

				//		F += calcAttractionForce(toPiece21, strength);*/

				//		float strength = /*exp(0.001 * lenPiece2)*/ lenPiece2 * 0.004f;
				//		F += calcAttractionForce(toPiece21, strength);
				//	}
				//}
				float dist = GetDist(piece->getCenter(), _center);
				Vector2f toPiece21 = getIdentityVector(GetDiff(piece->getCenter(), _center));

				float strength = exp(0.003 * dist) * 0.3f;
				if (!piece->isExcludedFromMerge())
				{
					strength *= 2.f;
				}
				F += calcAttractionForce(toPiece21, strength);

				Vector2f a = calcAcceleration(F, piece->getMass());
				Vector2f v = calcSpeed(a, diff, piece);
				updatePieceMove(piece, v, diff);
				cout << "piece " << num << ": " /* << F.x <<  "  "  << F.y*/<< piece->getCenter().x << "  " << piece->getCenter().y << endl;
				++num;
				/*++countPiece;
				cout << "piece " << countPiece << ": " << "xC: " << piece->getCenter().x << " yC: " << piece->getCenter().y << " isExcluded: " << piece->isExcludedFromMerge() << endl;*/
				
			}

			mergePieces(diff);

			vector<Vector2f> centers;
			float massSum = 0.f;
			Vector2f speedSum = { 0.f, 0.f };

			for (auto& [id, piece] : pieces)
			{
				centers.push_back(piece->getCenter());
				massSum += piece->getMass();
				speedSum += piece->getSpeed();
			}
			this->V = speedSum / (float)pieces.size();
			this->_mass = massSum;

			Vector2f mainCenter = findCyclicCenter(centers);
			setCenter(mainCenter);

			if (pieces.size() == 1)
			{
				/*pieces.begin()->second->setEatenState();*/
				pieces.clear();
				Splitted = false;
			}
			/*	cout << "BotCenterX: " << mainCenter.x << " BotCenterY: " << mainCenter.y << endl;*/
		}
		else
		{
			/*if (readyToWaekend)
			{
				setWeakened();
				readyToWaekend = false;
			}*/
			if (state == States::READY_TO_RESPAWN)
			{
				update(diff);
			}
			else if (state == States::LIVE)
			{
				updateMove(diff);
			}
		}

	}
	void Bot::hungry(int diff)
	{
		if (isSplitted())
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
		}	
	}

	void Bot::update(int diff)
	{
		elapsedRespTime += diff;
		if (elapsedRespTime >= RESPAWN_TIME)
		{
			state = States::READY_TO_LIVE;
			elapsedRespTime = 0;
		}
	}

	void Bot::udateMass()
	{
		float tmpMass = 0.f;
		for (const auto& piece : pieces)
		{
			if (piece.second->isLive())
			{
				tmpMass += piece.second->getMass();
			}
			else
			{
				tmpMass -= piece.second->getMass();
			}
			_mass = tmpMass;		
		}
	}

	bool Bot::createPiece(float& mass, Vector2f& center)
	{
		if (mass < 800.f)
			return false;
		for (int i = 0; i < 2; i++)
		{
			shared_ptr<Piece> p = make_shared<Piece>(this);
			pieces[p->getID()] = p;

			p->_mass = mass / 2;
			p->setExcludedFlag(true);
			p->setMaxV(0.5f / p->getRadius() * (i * 0.7 + 1));
			Vector2f toMouse1 = getIdentityVector(GetDiff(center, _Mouse));
			p->setV(V);
			p->setCenter(center + toMouse1 * (float)i);
		}
		return true;
	}

	float Bot::getMinDist(float radius)
	{
		const float k = 10;
		float sizeFactor = max(1.f, radius);
		return radius * k * (1.f / sizeFactor);
	}

	void Bot::updatePieceMove(std::shared_ptr<Piece>& piece, sf::Vector2f& v, int diff)
	{
		piece->setV(v);
		piece->Move(v * (float)diff);
	}

	bool Bot::canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2)
	{
		float dist = GetDist(p1->getCenter(), p2->getCenter());
		if (dist <= p1->getRadius() || dist <= p2->getRadius())
		{
			return true;
		}
		return false;
	}


	void Bot::together()
	{
		int count = 0;
		Vector2f co(0, 0);
		_mass = 0;
		V = { 0.f, 0.f };

		vector<Vector2f> centers;
		for (auto& piece : pieces)
		{
			V += piece.second->getSpeed();
			_mass += piece.second->_mass;
			centers.push_back(piece.second->getCenter());
		}

		co = findCyclicCenter(centers);
		setCenter(co);

		/*auto it = max_element(pieces.begin(), pieces.end(),
			[](auto a, auto b)
			{
				return b.second->getMass() > a.second->getMass();
			});
		for (auto& piece : pieces)
		{
			_mass += piece.second->_mass;
			co += GetDiff(piece.second->getCenter(), it->second->getCenter());
			V += piece.second->getSpeed();
			count++;
		}
		co /= (float)count;
		co += it->second->getCenter();

		V /= (float)count;
		setCenter(normalizeCoord(co));	*/
	}

	void Bot::mergePieces(int diff)
	{
		vector<Vector2f> centersIncludedPieces;
		for (const auto& piece : pieces)
		{
			if (!piece.second->isExcludedFromMerge())
			{
				centersIncludedPieces.push_back(piece.second->getCenter());
			}
		}
		Vector2f centerIncludedPieces = findCyclicCenter(centersIncludedPieces);
		/*cout << "IncludedCenterX : " << centerIncludedPieces.x << " IncludedCenterY: " << centerIncludedPieces.y << endl;*/
		bool mergedAny = true;

		while (mergedAny)
		{
			mergedAny = false;

			std::vector<std::shared_ptr<Piece>> newPieces;

			for (auto it = pieces.begin(); it != pieces.end(); )
			{
				auto& piece = it->second;
				bool merged = false;

				for (auto it2 = pieces.begin(); it2 != pieces.end(); )
				{
					auto& piece2 = it2->second;

					if (piece == piece2)
					{
						++it2;
						continue;
					}

					if (piece->isExcludedFromMerge() || piece2->isExcludedFromMerge())
					{
						++it2;
						continue;
					}

					Vector2f dir = getIdentityVector(GetDiff(piece->getCenter(), centerIncludedPieces));
					Vector2f F = calcAttractionForce(dir, 0.07f);
					Vector2f a = calcAcceleration(F, piece->getMass());
					Vector2f v = calcSpeed(a, diff, piece);
					updatePieceMove(piece, v, diff);

					if (canBeMerged(piece, piece2))
					{
						auto newPiece = std::make_shared<Piece>(this);


						vector<Vector2f> centers = { piece->getCenter(), piece2->getCenter() };
						Vector2f newCenter = findCyclicCenter(centers);

						Vector2f newSpeed = (piece->getSpeed() + piece2->getSpeed()) / 2.f;

						newPiece->setCenter(newCenter);
						newPiece->_mass = piece->getMass() + piece2->getMass();
						newPiece->setV(newSpeed);
						newPiece->setMaxV(0.6f / newPiece->getRadius());

						piece2->setEatenState();
						piece->setEatenState();

						it2 = pieces.erase(it2);
						it = pieces.erase(it);

						newPieces.push_back(newPiece);

						merged = true;
						mergedAny = true;
						break;
					}
					else
					{
						++it2;
					}
				}

				if (!merged)
				{
					++it;
				}
			}

			for (auto& np : newPieces)
			{
				pieces[np->getID()] = np;
			}
		}
	}

	bool Bot::checkEaten(MoveObject* other)
	{
		if (!isLive() || !other->isLive())
			return false;
		if (other->_mass > _mass * 1.2f && other->Eating(*this, -min(getRadius(), other->getRadius())))
		{
			setEatenState();
			return true;
		}
		else
		{
			return false;
		}
	}

	void Bot::eatPiece(const string& id)
	{
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
	}

	void Bot::Eat(Objects* obj)
	{
		obj->checkEaten(this);
	}

	void Bot::pieceToSides()
	{
		/*for (auto& piece : pieces)
		{
			Vector2f center = piece.second->getCenter();
			for (auto& piece2 : pieces)
			{
				if (&piece == &piece2)
				{
					continue;
				}
				float lenPiece2 = GetDist(piece.second->getCenter(), piece2.second->getCenter());
				float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
				if (lenPiece2 < sumRadius)
				{
					Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
					piece2.second->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
					piece.second->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
				}
			}
		}*/
		for (auto& piece : pieces)
		{
			Vector2f center = piece.second->getCenter();
			for (auto& piece2 : pieces)
			{
				if (&piece == &piece2)
				{
					continue;
				}

				if (piece.second->isExcludedFromMerge() || piece2.second->isExcludedFromMerge())
				{
					float lenPiece2 = GetDist(piece.second->getCenter(), piece2.second->getCenter());
					float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
					if (lenPiece2 < sumRadius)
					{
						Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
						piece2.second->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
						piece.second->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
					}
				}
			}
		}
		
	}



	//Vector2f Bot::calcAttractionForce(shared_ptr<Piece>& piece)
	//{
	//	Vector2f F(0, 0);
	//	Vector2f center = piece->getCenter();
	//	F += identityVector(GetDiff(_Mouse,center)) * 0.02f;
	//	for (auto& piece2 : pieces) {
	//		if (piece == piece2.second) {
	//			continue;
	//		}
	//		float lenPiece2 = GetDist(piece->getCenter(), piece2.second->getCenter());
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

	sf::Vector2f Bot::calcAttractionForce(sf::Vector2f& dir, float koef)
	{
		return dir * koef;
	}

	Vector2f Bot::calcAttractionForceToMouse(shared_ptr<Piece>& piece)
	{
		Vector2f dir = GetDiff(_Mouse, piece->getCenter());
		Vector2f force = identityVector(dir);
		return force * 0.01f;
	}

	Vector2f Bot::calcAcceleration(Vector2f F, float mass)
	{
		Vector2f a = F / mass;
		float lenA = GetLen(a);
		float maxA = 2.0 / mass;
		if (lenA > maxA) {
			a = a / lenA * (float)maxA;
		}
		return a;
	}

	Vector2f Bot::calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece)
	{
		Vector2f v{ 0.f,0.f };
		v = piece.get()->getSpeed() + a * (float)diff * (float)1.0;
		float lenV = GetLen(v);
		float maxV = piece.get()->getMaxV();
		/*float lenMouse = GetDist(_Mouse,getCenter());*/
		float lenMouse = GetDist(_Mouse, piece->getCenter());
		if (lenMouse < 40.f)
		{
			maxV *= (lenMouse / 40.f);
		}
		if (lenV > maxV) {
			/*v *= (float)maxV / lenV;*/
			v = v / lenV * maxV;
		}
		return v;
	}

	void Bot::setSplite()
	{
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
		createPiece(_mass, getCenter());
	}

	void Bot::setWeakened()
	{
		Splitted = true;
		count = _mass / 400;
		if (count > 8)
			count = 8;
		float remainMass = _mass;

		for (int i = 0; i < count; i++)
		{
			shared_ptr<Piece> p = make_shared<Piece>(this);
			pieces[p->getID()] = p;
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
			shared_ptr<Piece> p = make_shared<Piece>(this);
			pieces[p->getID()] = p;

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
	}

	void Bot::updateSplitTimer(float diff)
	{
		Timer -= diff;
		if (Timer < 0)
		{
			Timer = 5000;
			/*clearListPieces();*/
		}
	}

	void Bot::updatePiecePhysic(float diff, shared_ptr<Piece>& piece)
	{
		/*Vector2f F = calcAttractionForce(piece->getMass());
		Vector2f a = calcAcceleration(F, piece);
		Vector2f v = calcSpeed(a, diff, piece);
		piece->setV(v);
		piece->Move(piece->getSpeed() * diff);*/
	}

	void Bot::updateMove(float diff)
	{
		Vector2f vector = getCenter();
		Vector2f distMouse = GetDiff(vector, _Mouse);
		Vector2f d = getIdentityVector(distMouse);
		Vector2f a = d * 0.00014f;

		V += a * (float)diff;
		float lenV = GetLen(V);
		float len = GetLen(distMouse);
		float maxV = 0.04f;
		if (len < getRadius())
		{
			maxV *= (len / getRadius());
		}
		if (lenV > maxV)
		{
			V = V / lenV * maxV;
		}
		Move(V * diff);
	}

	void Bot::clearListPieces()
	{
		for (const auto& piece : pieces)
		{
			piece.second->setEatenState();
		}
		pieces.clear();
	}

	void Bot::reset()
	{
		_mass = minBotAndPlayerMass;
		Splitted = false;
		colB = (BotsColor)(rand() % 6);
		Timer = 50000;
		count = 0;
	}

	void Bot::setPosMouse(float x, float y)
	{
	
		_Mouse = normalizeCoord(Vector2f(x, y));
	}

	void Bot::setEatenState()
	{
		state = States::READY_TO_RESPAWN;
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

	bool Bot::isSplitted()
	{
		return Splitted;
	}
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
//			state = States::SPLITTED;
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
//			if (state == States::EATEN)
//				update(diff);
//			else if (state == States::LIVE)
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
//				state = States::LIVE;
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
//			/*state = States::READY_TO_NEXT_ACTION;
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
//			if (piece.second->isLive())
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
//		if (!isLive() || !other->isLive())
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
//	void Bot::setSplite()
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
//		state = States::READY_TO_RESPAWN;
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