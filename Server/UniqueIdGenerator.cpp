#include "UniqueIdGenerator.h"

#include <sstream>
#include <iomanip>

using namespace std;

UniqueIdGenerator::UniqueIdGenerator()
	:m_StartTime(chrono::steady_clock::now()), m_Count(0)
{
	random_device rd;
	m_Gen.seed(rd());
}

std::string UniqueIdGenerator::generateUniqueID()
{
	checkAndResetCount();

	int randomNum = m_Distribution(m_Gen);
	
	ostringstream oss;
	oss << hex << setw(6) << setfill('0') << m_Count << "-" << setw(4) << setfill('0') << randomNum;
	return oss.str();
}

void UniqueIdGenerator::checkAndResetCount()
{
	auto now = chrono::steady_clock::now();
	auto elapsed = now - m_StartTime;
	if (elapsed >= chrono::hours(20))
	{
		m_Count = 0;
		m_StartTime = now;
	}
	else
	{
		++m_Count;
	}
}
