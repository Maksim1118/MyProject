#pragma once
#include <SFML/Graphics.hpp>
#include "ObjectsServer.h"
#include "Colors.h"

#include "Respawn.h"

#include <iostream>

using namespace sf;
using namespace std;

namespace Server
{
	constexpr float _FoodMass = 25.0f;
	const float _FoodRadius = sqrt(_FoodMass);

	class Food : public Objects, public Respawn
	{
	public:
		Food(IRegistrator* iRegistrator);
		bool checkEaten(Objects& eatingObj) override;
		bool Eat(Objects& obj) override;
		void TimeElapsed(int diff);
		sf::FloatRect getMBR() const override;
		bool eatable = true;

		inline sf::Color getColor() const
		{
			return m_ListColors[m_ColorIndex];
		}
	protected:
		ColorsFood m_ListColors;
		int m_ColorIndex;
	private:
		FloatRect localBounds;
	private:
		void respawn() override;
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
	};
}

