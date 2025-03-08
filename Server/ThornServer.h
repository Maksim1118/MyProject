#pragma once
#include <SFML/Graphics.hpp>

#include "ObjectsServer.h"
#include "BotServer.h"

using namespace sf;
using namespace std;

namespace Server
{
	constexpr auto ThornSpriteSize = 10;
	constexpr float _ThornSpriteMass = 2500.f;
	const float _ThornSpriteR = sqrt(_ThornSpriteMass);

	class Thorn : public Objects
	{
	public:
		Thorn();
		void TimeElapsed(int diff);
		void update(int diff);
		bool checkEaten(MoveObject* player) override;
	private:
		int curTime;
		int respawnTime;
	};
}


