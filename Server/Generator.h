#pragma once

#include <random>
#include<type_traits>

static std::random_device rd{};
static std::mt19937 gen{rd()};

template<typename T> 
inline T genNumber(const T min, const T max)
{
	static_assert(std::is_arithmetic<T>::value, "T must be a numeric type\n");

	if constexpr (std::is_integral<T>::value)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(gen);
	}
	else if constexpr (std::is_floating_point<T>::value)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(gen);
	}
	else
	{
		static_assert(always_false<T>::value, "Unsupported type", "undefined type\n");
	}
}