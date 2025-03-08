#pragma once
#include "FoodServer.h"
#include "MoveObjectServer.h"

namespace Server
{
	class Feed : virtual public Food, virtual public MoveObject
	{
	public:
		Feed();
		void TimeElapsed(int diff);
		void setV(Vector2f& newV);
		void setParentCenter(Vector2f& newCenter);
		void setParentRadius(float newRadius);
	private:
		Vector2f parentCenter;
		float parentRadius;
	};
}


