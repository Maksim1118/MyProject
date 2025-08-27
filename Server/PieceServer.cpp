#include "PieceServer.h"
#include "BotServer.h"

#include <iostream>

namespace Server
{
	Piece::Piece(Bot* bot) : MoveObject(Vector2f(0, 0), 400.f), m_BotParent(bot)
	{
		V = { 0.f,0.f };
		maxV = 0.001f;
		timerActive = 0.f;
		excludedFromMerge = false;
		m_ParentColor = static_cast<int>(m_BotParent->colB);
	}


	void Piece::TimeElapsed(int diff)
	{	
		if (excludedFromMerge)
		{
			updateTimerActive(diff);
		}
	}

	void Piece::setMaxV(float newMaxV)
	{
		maxV = newMaxV;
	}

	bool Piece::checkEaten(MoveObject* other)
	{
		if (!isLive() || !other->isLive())
			return false;
		if (other->_mass > _mass * 1.2f && other->Eating(*this, -min(getRadius(), other->getRadius())))
		{
			setEatenState();

			if (m_BotParent)
			{
				m_BotParent->eatPiece(getID());
			}
			return true;
		}
		return false;
	}

	void Piece::Eat(Objects* obj)
	{
		obj->checkEaten(this);
	}

	void Piece::updateTimerActive(const int diff)
	{
		timerActive -= diff;
		if (timerActive <= 0)
		{
			excludedFromMerge = false;
			timerActive = 0;
		}
	}

	void Piece::setEatenState()
	{
		state = States::READY_TO_REMOVE;
	}

	bool Piece::isExcludedFromMerge() const
	{
		return excludedFromMerge;
	}

	void Piece::setExcludedFlag(const bool exclude)
	{
		excludedFromMerge = exclude;
		timerActive = 5000;
	}

	void Piece::setV(Vector2f newV)
	{
		V = newV;
	}

	float Piece::getMaxV()
	{
		return maxV;
	}
	Bot* Piece::getParentPointer()
	{
		return m_BotParent;
	}
}
