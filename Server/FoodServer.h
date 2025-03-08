#pragma once
#include <SFML/Graphics.hpp>
#include "ObjectsServer.h"

#include <iostream>
using namespace sf;
using namespace std;

namespace Server
{
	constexpr float _FoodMass = 25.0f;
	const float _FoodRadius = sqrt(_FoodMass);

	class Food : virtual public Objects
	{
	public:
		Food();
		void TimeElapsed(int diff);
		void update(int diff);
		bool checkEaten(MoveObject* obj) override;
		int colorNum;
		bool eatable = true;
	protected:
		int respawnTime;
		int curTime;
	};
}

