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
		void setEatenState() override;
	private:
		Vector2f parentCenter;
		float parentRadius;
		int colorNum;
		int respawnTime;
		int curTime;
		bool m_IsUnderHero;
	};
}


