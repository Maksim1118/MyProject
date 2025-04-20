#include "BotServer.h"

#include <random>

namespace Server
{
	Bot::Bot(string text) :MoveObject(Vector2f(0, 0), 400.000f), respawnTime(5000), curTime(0)
	{
		name = text;
		prevMass = _mass;
		Splitted = false;
		colB = (BotsColor)(rand() % 6);
		Timer = 0;
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

	void Bot::TimeElapsed(int diff)
	{
		hungry(diff);
		if (Splitted == true)
		{
			pieceToSides();
			for (auto& it = pieces.begin(); it != pieces.end();) {
				if ((*it)->readyToWaekend)
				{
					(*it)->readyToWaekend = false;
					if (setPieceWeakend(it))
					{
						it = pieces.erase(it);
						continue;
					}
				}
				Vector2f F = calcRepulsiveForce((*it));
				Vector2f a = calcAcceleration(F, (*it));
				Vector2f v = calcSpeed(a, diff, (*it));
				(*it).get()->setV(v);
				(*it).get()->Move((*it).get()->getSpeed() * (float)diff);
				it++;
			}
			together();	
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
				Vector2f vector = getCenter();

				Vector2f d = getIdentityVector(_Mouse - vector);
				Vector2f a = d * 0.00014f;

				V += a * (float)diff;
				float lenV = GetLen(V, Vector2f(0, 0));
				float len = GetLen(_Mouse - vector);
				float maxV = 0.04f;
				if (len < getRadius())
				{
					maxV *= (len / getRadius());
				}
				if (lenV > maxV)
				{
					V = V / lenV * maxV;
				}
				setCenter(getCenter() + V * (float)diff);
			}
		}

		if (Splitted == true)
		{
			Timer -= diff;
			if (Timer < 0)
			{
				together();
				Splitted = false;
			}
		}
	}

	void Bot::hungry(int diff)
	{
		if (isSplitted())
		{
			for (auto& p : pieces)
			{
				if(p->_mass > 400/ pieces.size())
					p->_mass *= exp(-0.000004f * float(diff));
				else
					p->_mass = 400/pieces.size();
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
		curTime += diff;
		if (curTime >= respawnTime)
		{
			state = States::READY_TO_LIVE;
			curTime = 0;
			_mass = 0;
			V = { 0.f, 0.f };
			_center = { 0.f, 0.f };
			_Mouse = { 0.f, 0.f };
		}
	}

	void Bot::udateMass()
	{
		float tmpMass = 0.f;
		for (const auto& piece : pieces)
		{
			if (piece->state == States::LIVE)
			{
				tmpMass += piece->getMass();
			}
			else if (piece->state == States::EATEN)
			{
				tmpMass -= piece->getMass();
			}
			_mass = tmpMass;		
		}
	}

	bool Bot::createPiece(float& mass, Vector2f& center)
	{
		if (mass < 800.f)
			return false;
		Timer = 5000;
		for (int i = 0; i < 2; i++)
		{
			Piece* p = new  Piece();
			pieces.emplace_back(p);

			p->_mass = mass / 2;
			p->setMaxV(0.7f / p->getRadius() * (i + 1));
			Vector2f toMouse1 = getIdentityVector(_Mouse - center);
			p->setV(toMouse1 * p->getMaxV());
			p->setCenter(center + toMouse1 * (float)i);
		}
		return true;
	}

	void Bot::together()
	{
		int count = 0;
		Vector2f co(0, 0);
		_mass = 0;
		V = { 0.f, 0.f };
		for (auto& piece : pieces)
		{
			_mass += piece.get()->_mass;
			co += piece->getCenter();
			V += piece->getSpeed();
			count++;
		}
		co /= (float)count;
		V /= (float)count;
		setCenter(co);	
	}

	bool Bot::checkEaten(MoveObject* other)
	{
		if (other->state == States::EATEN || state == States::EATEN)
			return false;
		if (isSplitted())
		{
			for (auto& piece : pieces)
			{
				if (other->_mass > piece->_mass * 1.2f)
				{
					other->Eating(*piece, -min(piece->getRadius(), other->getRadius()));
					if (pieces.empty())
						state = States::EATEN;
					return true;
				}
			}
		}
		else
		{
			if (other->_mass > _mass * 1.2f)
			{
				other->Eating(*this, -min(getRadius(), other->getRadius()));
				return true;
			}
		}
		return false;
	}

	void Bot::Eat(Objects* obj)
	{
		if (isSplitted())
		{
			for (auto Piece = pieces.begin(); Piece != pieces.end(); ++Piece)
			{
				obj->checkEaten((*Piece).get());
			}
		}
		else
		{
			obj->checkEaten(this);
		}
	}

	void Bot::pieceToSides()
	{
		for (auto& piece : pieces)
		{
			Vector2f center = piece->getCenter();
			for (auto& piece2 : pieces)
			{
				if (&piece == &piece2)
				{
					continue;
				}
				float lenPiece2 = GetLen(piece->getCenter(), piece2->getCenter());
				float sumRadius = piece->getRadius() + piece2->getRadius();
				if (lenPiece2 < sumRadius)
				{
					Vector2f toPiece21 = getIdentityVector(piece2->getCenter() - center);
					piece2->Move(toPiece21 * (sumRadius - lenPiece2) / (float)2);
					piece->Move(-toPiece21 * (sumRadius - lenPiece2) / (float)2);
				}
			}
		}
	}

	Vector2f Bot::calcRepulsiveForce(shared_ptr<Piece>& piece)
	{
		Vector2f F(0, 0);
		Vector2f center = piece->getCenter();
		F += getIdentityVector(_Mouse - center);
		for (auto& piece2 : pieces) {
			if (piece == piece2) {
				continue;
			}
			float lenPiece2 = GetLen(piece->getCenter(), piece2->getCenter());
			float sumRadius = piece->getRadius() + piece2->getRadius();
			if (lenPiece2 > sumRadius)
			{
				Vector2f toPiece21 = getIdentityVector(piece2->getCenter() - center);
				F += toPiece21 * (float)0.01;
			}
		}
		return F;
	}

	Vector2f Bot::calcAcceleration(Vector2f F, shared_ptr<Piece>& piece)
	{
		Vector2f a = F / piece->_mass;
		float lenA = GetLen(a);
		float maxA = 2.0 / piece->_mass;
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
		float lenMouse = GetLen(_Mouse - getCenter());
		if (lenMouse < 40)
		{
			maxV *= (lenMouse / 40);
		}
		if (lenV > maxV) {
			v *= (float)maxV / lenV;
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
			list<shared_ptr<Piece>> oldPieces = pieces;
			pieces.clear();
			for (auto&p = oldPieces.begin(); p != oldPieces.end();)
			{
				if (pieces.size()+oldPieces.size() >= 8)
				{
					pieces.push_back(*p);
					p++;
					continue;
				}
				if (!createPiece((*p)->_mass, (*p)->getCenter()))
					pieces.push_back(*p);
				p = oldPieces.erase(p);		
			}
			return;
		}
		Splitted = true;
		pieces.clear();
		createPiece(_mass, getCenter());
	}

	void Bot::setWeakened()
	{
		Splitted = true;
		Timer = 5000;
		count = _mass / 400;
		if (count > 8)
			count = 8;
		float remainMass = _mass;
		pieces.clear();
		for (int i = 0; i < count; i++)
		{
			Piece* p = new Piece();
			pieces.emplace_back(p);
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
		}
	}

	bool Bot::setPieceWeakend(list<shared_ptr<Piece>>::iterator & obj)
	{
		if ((*obj).get()->_mass < 400)
			return false;
		if (pieces.size() >= 8)
			return false;
		count = (*obj).get()->_mass / 400;
		if (count + pieces.size() > 8)
			count = 8 - pieces.size() + 1;
		if (count == 1)
			return false;
		float remainMass = (*obj).get()->_mass;
		for (int i = 0; i < count; i++)
		{
			Piece* p = new Piece();
			pieces.emplace_back(p);

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

			p->setCenter((*obj).get()->getCenter());
		}
		return true;	
	}

	void Bot::setPosMouse(float x, float y)
	{
		_Mouse = Vector2f(x, y);
	}

	string Bot::getName()
	{
		return name;
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