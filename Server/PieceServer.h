#pragma once
#include "MoveObjectServer.h"
#include "ObjectsServer.h"

namespace Server
{
	class Piece : public MoveObject
	{
	public:
		Piece();
		void TimeElapsed(int diff);
		void setV(Vector2f newV);
		void setMaxV(float newMaxV);
		bool checkEaten(MoveObject* other)override { return false; };
		float getMaxV();
	private:
		float maxV;
	};

}

