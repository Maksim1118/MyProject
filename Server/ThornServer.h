#pragma once
#include <SFML/Graphics.hpp>

#include "ObjectsServer.h"
#include "Respawn.h"

using namespace sf;
using namespace std;

namespace Server
{
	constexpr auto ThornSpriteSize = 10;
	constexpr float _ThornSpriteMass = 2500.f;
	const float _ThornSpriteR = sqrt(_ThornSpriteMass);

	class Thorn : public Objects, public Respawn
	{
	public:
		Thorn(IRegistrator* iRegistrator);
		bool Eat(Objects& obj) override;
		bool checkEaten(Objects& eatingObj) override;
		void TimeElapsed(int diff);
	private:
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
		void respawn() override;
	};
}


