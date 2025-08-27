#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>


namespace Server
{
	class Thorn;
	class Food;
	class Hero;
	class Bot;
	class Feed;
	class Piece;

	class MapSegment
	{
	public:

		template<typename T>
		std::unordered_map<std::string, std::shared_ptr<T>>& getSegmentList();

		template<typename T>
		const std::unordered_map<std::string, std::shared_ptr<T>>& getSegmentList() const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Thorn>> thorns;
		std::unordered_map<std::string, std::shared_ptr<Food>> foods;
		std::unordered_map<std::string, std::shared_ptr<Hero>> heroes;
		std::unordered_map<std::string, std::shared_ptr<Bot>> bots;
		std::unordered_map<std::string, std::shared_ptr<Feed>> feeds;
		std::unordered_map<std::string, std::shared_ptr<Piece>> pieces;
	};

	template<>
	inline const std::unordered_map<std::string, std::shared_ptr<Thorn>>& MapSegment::getSegmentList<Thorn>() const
	{
		return thorns;
	}

	template<>
	inline std::unordered_map<std::string, std::shared_ptr<Thorn>>& MapSegment::getSegmentList<Thorn>()
	{
		return thorns;
	}

	template<>
	inline const std::unordered_map<std::string, std::shared_ptr<Bot>>& MapSegment::getSegmentList<Bot>()const
	{
		return bots;
	}

	template<>
	inline std::unordered_map<std::string, std::shared_ptr<Bot>>& MapSegment::getSegmentList<Bot>()
	{
		return bots;
	}

	template<>
	inline const std::unordered_map<std::string, std::shared_ptr<Food>>& MapSegment::getSegmentList<Food>() const
	{
		return foods;
	}

	template<>
	inline std::unordered_map<std::string, std::shared_ptr<Food>>& MapSegment::getSegmentList<Food>()
	{
		return foods;
	}

	template<>
	inline const std::unordered_map<std::string, std::shared_ptr<Piece>>& MapSegment::getSegmentList<Piece>() const
	{
		return pieces;
	}

	template<>
	inline std::unordered_map<std::string, std::shared_ptr<Piece>>& MapSegment::getSegmentList<Piece>()
	{
		return pieces;
	}

	template<>
	inline const std::unordered_map<std::string, std::shared_ptr<Hero>>& MapSegment::getSegmentList<Hero>() const
	{
		return heroes;
	}

	template<>
	inline std::unordered_map<std::string, std::shared_ptr<Hero>>& MapSegment::getSegmentList<Hero>()
	{
		return heroes;
	}

	struct segmentInterval
	{
		short left = 0;
		short right = 0;
		short top = 0;
		short bottom = 0;
	};

	template<typename T>
	struct SegmentInfo
	{
		std::string id;
		std::shared_ptr<T> item;
		sf::Vector2i indices;
	};

}
