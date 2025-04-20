#include "PieceServer.h"

#include <iostream>

namespace Server
{
	Piece::Piece() : MoveObject(Vector2f(0, 0), 400.f)
	{
		V = { 0.f,0.f };
		maxV = 0.001f;
	}

	void Piece::TimeElapsed(int diff)
	{
		
	}

	void Piece::setMaxV(float newMaxV)
	{
		maxV = newMaxV;
	}

	void Piece::setV(Vector2f newV)
	{
		V = newV;
	}

	float Piece::getMaxV()
	{
		return maxV;
	}
}
