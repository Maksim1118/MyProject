#pragma once
#include <SFML/Graphics.hpp>
#include "ObjectsServer.h"
#include "Colors.h"

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
		bool checkEaten(MoveObject* obj) override;
		void setEatenState() override;
		bool eatable = true;

		inline sf::Color getColor() const
		{
			return m_ListColors[m_ColorIndex];
		}
	protected:
		ColorsFood m_ListColors;
		int m_ColorIndex;
		int respawnTime;
		int elapsedRespTime;
		bool m_isRespawnState;

	private:
		void update(int diff);
	};
}

