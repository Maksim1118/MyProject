#pragma once

#include <list>
#include <iterator>
#include <functional>
using namespace std;

namespace
{
	
	template<typename T>
	void mergeLists(list<T*>& ListObj,
		typename list<T*>::iterator& left, 
		typename list<T*>::iterator& mid, 
		typename list<T*>::iterator& right,
		function<bool(T*, T*)> argument)
	{
		list<T*> merged;
		auto leftIt = left;
		auto rightIt = mid;

		while (leftIt != mid && rightIt != right)
		{
			if (!argument(*rightIt, *leftIt))
			{
				merged.push_back(*leftIt);
				++leftIt;
			}
			else
			{
				merged.push_back(*rightIt);
				++rightIt;
			}
		}

		while (leftIt != mid)
		{
			merged.push_back(*leftIt);
			++leftIt;
		}

		while (rightIt != right)
		{
			merged.push_back(*rightIt);
			++rightIt;
		}

		auto mergeIt = merged.begin();
		for (auto it = left; it != right; ++it, ++mergeIt) {
			*it = *mergeIt;
		}
	}

	template<typename T>
	void insertionSort(list<T*>& ListObj,
		typename list<T*>::iterator& left,
		typename list<T*>::iterator& right, 
		function<bool(T*, T*)> argument)
	{
		for (auto it = next(left); it != right; ++it)
		{
			auto key = *it;
			auto j = it;
			while (j != left && argument(key,*prev(j)))
			{
				*j = *prev(j);
				--j;
			}
			*j = key;
		}
	}

	int getMinRun(int n)
	{
		int r = 0;
		while (n >= 64)
		{
			r |= (n & 1);
			n >>= 1;
		}
		return n + r;
	}

}

template<typename T>
void timSort(list<T*>& ListObj, function<bool(T*, T*)> argument)
{
	if (ListObj.empty())
		return;
	int minRun = getMinRun(static_cast<int>(ListObj.size()));

	for (auto it = ListObj.begin(); it != ListObj.end();)
	{
		auto nextIt = next(it, min(minRun, static_cast<int>(distance(it, ListObj.end()))));
		insertionSort(ListObj, it, nextIt, argument);
		it = nextIt;
	}

	for (int size = minRun; size < ListObj.size(); size *= 2)
	{
		for (auto it = ListObj.begin(); it != ListObj.end();)
		{
			auto left = it;
			auto mid = next(it, min(size, static_cast<int>(distance(it, ListObj.end()))));
			if (mid == ListObj.end())
			{
				break;
			}
			auto right = next(mid, min(size, static_cast<int>(distance(mid, ListObj.end()))));

			if (mid != right)
			{
				mergeLists(ListObj, left, mid, right,argument);
			}
			it = right;
		}
	}
}

template<typename T>
typename list<T*>::iterator findPos(
	list<T*>& ListObj, 
	const T* element,
	function<bool(const T*, const T*)> argument)
{
	auto left = ListObj.begin();
	auto right = ListObj.end();

	while (left != right)
	{
		auto mid = next(left, distance(left, right)/2);

		if (argument(*mid, element))
		{
			left = next(mid);
		}
		else
		{
			right = mid;
		}
	}
	return left;
}