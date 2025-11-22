#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "BaseOld.h"
#include "MapSegment.h"
#include "ObjectsServer.h"

#include "IRegistrator.h"

namespace Server
{
	class Thorn;
	class Food;
	class Hero;
	class Bot;
	class Feed;

	class RTree;

	class Map: public IRegistrator
	{
	public:

		Map();

		void TimeElapsed(int diff);

		bool spawn(float radius, sf::Vector2f& spawnPosition) override;
		void registerAuxiliary(std::shared_ptr<Objects> obj) override;
		void unregisterAuxiliary(std::shared_ptr<Objects> obj) override;
		void moveObj(std::shared_ptr<Objects> obj) override;
		std::vector<std::shared_ptr<Objects>> getNearObjects(const sf::FloatRect& area) const override;

		void add(std::shared_ptr<Objects> obj);
		std::shared_ptr<Objects> findObj(const std::string& id);
		void createSceneObjects();
	private:
		bool isQuadFree(const sf::FloatRect& quad, const float radius);
		std::pair<sf::Vector2f, bool> findFreePositionInQuad(const sf::FloatRect& quad, const float radius, int currentDepth);

		template<typename T, typename... Args>
		std::shared_ptr<T> createObj(Args... args);

		template<typename T>
		void fillObjects(const size_t listSize, std::function<std::shared_ptr<T>()> factory);
	private:
		std::unordered_map <std::string, std::shared_ptr<Objects>> listObjects;
		std::unordered_set<std::string> toBeRemovedObjects;
		std::unique_ptr<RTree> m_rTree;
		bool initialized;
		
	};
}