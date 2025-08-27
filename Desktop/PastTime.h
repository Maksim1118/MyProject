#pragma once

#include <SFML/System.hpp>

class PastTime
{
public:
	void update()
	{
		sf::Time currTime = m_Clock.getElapsedTime();

		m_DeltaTime = currTime.asMilliseconds() - m_LastTime.asMilliseconds();

		m_LastTime = currTime;
	}

	inline int getDeltaTime()
	{
		return m_DeltaTime;
	}
private:
	sf::Clock m_Clock;
	sf::Time m_LastTime = sf::Time::Zero;
	int m_DeltaTime = 0;
};