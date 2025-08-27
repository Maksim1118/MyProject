#pragma once

#include <random>
#include <string>
#include <chrono>

class UniqueIdGenerator
{
public:
	static inline std::string generateID()
	{
		return getInstance().generateUniqueID();
	}

private:
	UniqueIdGenerator();

	std::string generateUniqueID();
	void checkAndResetCount();
	
	static inline UniqueIdGenerator& getInstance()
	{
		static UniqueIdGenerator instance;
		return instance;
	}

	UniqueIdGenerator(const UniqueIdGenerator&) = delete;
	UniqueIdGenerator& operator=(const UniqueIdGenerator&) = delete;

	std::mt19937 m_Gen;
	std::uniform_int_distribution<int> m_Distribution{ 0, 0xFFFF };
	long int m_Count = 0;
	std::chrono::steady_clock::time_point m_StartTime;
};