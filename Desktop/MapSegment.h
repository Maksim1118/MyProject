#pragma once

#include <unordered_map>
#include <string>
#include <list>
#include "Objects.h"

class Hero;
class Bot;
class Thorn;
class Food;
class Feed;

struct mapSegment
{
	std::unordered_map<std::string, Hero*> players;
	std::unordered_map<std::string, Bot*> bots;
	std::unordered_map<std::string,Thorn*> thorns;
	std::unordered_map<std::string, Food*> foods;
	std::unordered_map<std::string, Feed*> feeds;
	std::list<Objects*> objects;

	std::vector<std::string> listOldIds;
};

struct segmentInterval
{
	short left;
	short right;
	short top;
	short bottom;

	bool inline isInterval(short firstInd, short secondInd)
	{
		return firstInd >= left && firstInd < right && secondInd >= top && secondInd < bottom;
	}
};