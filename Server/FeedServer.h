#pragma once
#include "FoodServer.h"
#include "MoveObjectServer.h"

namespace Server
{
	class Feed : public MoveObject
	{
	public:
		Feed();
		void TimeElapsed(int diff);
		void setV(Vector2f& newV);
		void setParentCenter(Vector2f& newCenter);
		void setParentRadius(float newRadius);
		bool checkEaten(MoveObject* obj) override;
	private:
		Vector2f parentCenter;
		float parentRadius;
		int colorNum;
		bool eatable;
		int respawnTime;
		int curTime;
		bool m_IsUnderHero;
	};
}


