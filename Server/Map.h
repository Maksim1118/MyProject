#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "BaseOld.h"
#include "MapSegment.h"

constexpr int segmentRows = 10;
constexpr int segmentCol = 10;

const sf::Vector2f segmentSize = { fieldWidth / segmentRows, fieldHeight / segmentCol };

namespace Server
{
	class Thorn;
	class Food;
	class Hero;
	class Bot;
	class Feed;

	class Map
	{
	public:

		template<typename T>
		const std::unordered_map <std::string, std::shared_ptr<T>>& getGlobalList() const;

		template<typename T>
		std::unordered_map <std::string, std::shared_ptr<T>>& getGlobalList();

		template<typename T>
		bool updateSegments(const std::string& id, const std::shared_ptr<T>& item);
		template<typename T>
		bool findObjInSegments(const std::string& id, SegmentInfo<T>& InfoOut);
		template<typename T>
		bool removeFromSegment(const sf::Vector2i& indices, const std::string& id);
		template<typename T>
		bool addToSegment(const sf::Vector2i& indices, const std::string& id, const std::shared_ptr<T>& item);
		template<typename T>
		bool addToGlobalList(const std::string& id, const std::shared_ptr<T>& item);
		template<typename T>
		bool eraseFromGlobalList(const std::string& id);

		bool isInterval(const short firstInd, const short secondInd, const segmentInterval& interval, const sf::Vector2f& sizeParamObj);
		std::pair<int, int> calcBoundsCyclic(float pos, float sizeParamObj, float segmentSize, int segCount) const;
		int normalize(int ind, int segCount) const;

		segmentInterval getSegmentInterval(sf::Vector2f center, sf::Vector2f sizeParamObj) const;



		sf::Vector2i Map::findSegmentIndices(sf::Vector2f pos);
		std::vector<sf::Vector2i> objectInSegments(sf::Vector2f center, float radius);

		const MapSegment& getSegment(sf::Vector2i indices) const;
		MapSegment& Map::getSegment(sf::Vector2i indices);

	private:
		template<typename T>
		bool validateSegmentInfo(const SegmentInfo<T>& infoSeg);
		bool isIndicesOut(const sf::Vector2i indices) const;
	private:
		std::unordered_map <std::string, std::shared_ptr<Thorn>> listThorns;
		std::unordered_map <std::string, std::shared_ptr<Food>> listFoods;
		std::unordered_map <std::string, std::shared_ptr<Hero>> listHeroes;
		std::unordered_map <std::string, std::shared_ptr<Bot>> listBots;
		std::unordered_map <std::string, std::shared_ptr<Feed>> m_ListFeeds;

		MapSegment m_mapSegments[segmentRows][segmentCol];
	};

	template<typename T>
	inline bool Map::addToGlobalList(const std::string& id, const std::shared_ptr<T>& item)
	{
		if (id.empty() || item == nullptr)
		{
			return false;
		}
		auto& list = getGlobalList<T>();
		list[id] = item;
		return true;
	}

	template<>
	inline const std::unordered_map <std::string, std::shared_ptr<Thorn>>& Map::getGlobalList<Thorn>() const
	{
		return listThorns;
	}

	template<>
	inline std::unordered_map <std::string, std::shared_ptr<Thorn>>& Map::getGlobalList<Thorn>()
	{
		return listThorns;
	}

	template<>
	inline const std::unordered_map <std::string, std::shared_ptr<Food>>& Map::getGlobalList<Food>() const
	{
		return listFoods;
	}

	template<>
	inline std::unordered_map <std::string, std::shared_ptr<Food>>& Map::getGlobalList<Food>()
	{
		return listFoods;
	}

	template<>
	inline const std::unordered_map <std::string, std::shared_ptr<Hero>>& Map::getGlobalList<Hero>() const
	{
		return listHeroes;
	}

	template<>
	inline std::unordered_map <std::string, std::shared_ptr<Hero>>& Map::getGlobalList<Hero>()
	{
		return listHeroes;
	}

	template<>
	inline const std::unordered_map <std::string, std::shared_ptr<Bot>>& Map::getGlobalList<Bot>() const
	{
		return listBots;
	}

	template<>
	inline std::unordered_map <std::string, std::shared_ptr<Bot>>& Map::getGlobalList<Bot>()
	{
		return listBots;
	}

	template<>
	inline const std::unordered_map <std::string, std::shared_ptr<Feed>>& Map::getGlobalList<Feed>() const
	{
		return m_ListFeeds;
	}

	template<>
	inline std::unordered_map <std::string, std::shared_ptr<Feed>>& Map::getGlobalList<Feed>()
	{
		return m_ListFeeds;
	}

	template<typename T>
	inline bool Map::validateSegmentInfo(const SegmentInfo<T>& infoSeg)
	{
		if (infoSeg.id.empty())
		{
			std::cerr << "error: id is empty\n";
			return false;
		}

		if (infoSeg.indices.x < 0 || infoSeg.indices.y < 0
			|| infoSeg.indices.x >= segmentCol || infoSeg.indices.y >= segmentRows)
		{
			std::cerr << "error: indices out of range\n";
			return false;
		}

		if (!infoSeg.item)
		{
			std::cerr << "error: pointer is not readed\n";
			return false;
		}

		return true;
	}

	template<typename T>
	inline bool Map::removeFromSegment(const sf::Vector2i& indices, const std::string& id)
	{
		if (isIndicesOut(indices))
		{
			return false;
		}
		m_mapSegments[indices.x][indices.y].getSegmentList<T>().erase(id);
		return true;
	}

	template<typename T>
	inline bool Map::addToSegment(const sf::Vector2i& indices, const std::string& id, const std::shared_ptr<T>& item)
	{
		if (isIndicesOut(indices))
		{
			return false;
		}
		m_mapSegments[indices.x][indices.y].getSegmentList<T>()[id] = item;
		return true;
	}

	template<typename T>
	inline bool Map::updateSegments(const std::string& id, const std::shared_ptr<T>& item)
	{

		if (!item)
		{
			std::cerr << "error: memory read\n";
			return false;
		}

		sf::Vector2i indices = findSegmentIndices(item->getCenter());

		SegmentInfo<T> prevInfo;
		bool isFound = findObjInSegments(id, prevInfo);
		if (isFound)
		{
			if (validateSegmentInfo(prevInfo))
			{
				if (prevInfo.indices != indices)
				{
					bool isRemove = removeFromSegment<T>(prevInfo.indices, id);
					if (!isRemove)
					{
						std::cerr << "error remove from segment\n";
					}

					bool isAdd = addToSegment(indices, id, item);
					if(!isAdd)
					{
						std::cerr << "error add in segment\n";
					}
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			bool isAdd = addToSegment(indices, id, item);
			if (!isAdd)
			{
				std::cerr << "error add in segment\n";
			}
		}
		return true;
	}

	template<typename T>
	inline bool Map::eraseFromGlobalList(const std::string& id)
	{
		auto& list = getGlobalList<T>();
		auto it = list.find(id);
		if (it != list.end())
		{
			list.erase(it);
			return  true;
		}
		return false;
	}

	template<typename T>
	inline bool Map::findObjInSegments(const std::string& id, SegmentInfo<T>& InfoOut)
	{
		for (size_t x = 0; x < segmentCol; ++x)
		{
			for (size_t y = 0; y < segmentRows; ++y)
			{
				auto& container = m_mapSegments[x][y].getSegmentList<T>();
				auto it = container.find(id);
				if (it != container.end())
				{
					InfoOut.id = id;
					InfoOut.indices = sf::Vector2i(static_cast<int>(x), static_cast<int>(y));
					InfoOut.item = it->second;
					return true;
				}
			}
		}
		return false;
	}
}