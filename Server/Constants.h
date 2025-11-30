#pragma once

namespace Server
{
	namespace MapConstants
	{
		constexpr float mapWidth = 2500.0f;
		constexpr float mapHeight = 2500.0f;
	}

	namespace GameConstants
	{
		constexpr float MIN_ENTITY_MASS = 400.0f;
		constexpr float THORN_MASS = 2500.0f;
		constexpr float FOOD_MASS = 25.0f;
		constexpr float FEED_MASS = 25.0f;

		constexpr size_t MAX_COUNT_PIECES = 8;
		constexpr float IMPULSE_FORCE = 1e-2f;
		constexpr float FRICTION = 0.03f;
		constexpr float HUNGRY_KOEF = -4e-6;
		constexpr float FORCE_KOEF = 40.f;
		constexpr float MAX_V = 2.0f; //2.0f
	}

	namespace ServerConstants
	{
		constexpr double PI = 3.14159265358979323846;
		constexpr int TIMER_SENT_STATIC_DATA = 3000;
		constexpr float EPSILON = 1e-6;
	}
}