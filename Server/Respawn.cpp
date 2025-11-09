#include "Respawn.h"

Respawn::Respawn(int timeDuration)
	:m_timeDuration(timeDuration), m_currentTime(timeDuration)
{
}

void Respawn::update(int diff)
{
	if (m_currentTime > 0.f)
	{
		m_currentTime -= diff;
	}
}

void Respawn::reset()
{
	m_currentTime = m_timeDuration;
}

bool Respawn::isReady() const
{
	return m_currentTime <= 0;
}
