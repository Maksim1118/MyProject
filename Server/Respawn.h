#pragma once

class Respawn
{
public:
	Respawn(int timeDuration);
	void update(int diff);
	void reset();
	virtual void respawn() = 0;
	bool isReady() const;
private:
	int m_timeDuration;
	int m_currentTime;
};