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

//constexpr int segmentRows = 10;
//constexpr int segmentCol = 10;
//
//const sf::Vector2f segmentSize = { fieldWidth / segmentRows, fieldHeight / segmentCol };

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

		/*template<typename T>
		const std::unordered_map <std::string, std::shared_ptr<T>>& getList() const;

		template<typename T>
		std::unordered_map <std::string, std::shared_ptr<T>>& getList();

		template<typename T>
		void addInList(const std::string& id, const std::shared_ptr<T>& item);

		void removeInList(const std::string& id, ObjectType type);*/

		//template<typename T>
		//bool updateSegments(const std::string& id, const std::shared_ptr<T>& item);
		//template<typename T>
		//bool findObjInSegments(const std::string& id, SegmentInfo<T>& InfoOut);
		//template<typename T>
		//bool removeFromSegment(const sf::Vector2i& indices, const std::string& id);
		//template<typename T>
		//bool addToSegment(const sf::Vector2i& indices, const std::string& id, const std::shared_ptr<T>& item);
		//template<typename T>
		//bool eraseFromGlobalList(const std::string& id);

		/*bool isInterval(const short firstInd, const short secondInd, const segmentInterval& interval, const sf::Vector2f& sizeParamObj);
		std::pair<int, int> calcBoundsCyclic(float pos, float sizeParamObj, float segmentSize, int segCount) const;
		int normalize(int ind, int segCount) const;*/

	/*	segmentInterval getSegmentInterval(sf::Vector2f center, sf::Vector2f sizeParamObj) const;



		sf::Vector2i Map::findSegmentIndices(sf::Vector2f pos);
		std::vector<sf::Vector2i> objectInSegments(sf::Vector2f center, float radius);

		const MapSegment& getSegment(sf::Vector2i indices) const;
		MapSegment& Map::getSegment(sf::Vector2i indices);*/

	private:
		bool isQuadFree(const sf::FloatRect& quad, const float radius);
		std::pair<sf::Vector2f, bool> findFreePositionInQuad(const sf::FloatRect& quad, const float radius, int currentDepth);

		template<typename T, typename... Args>
		std::shared_ptr<T> createObj(Args... args);

		template<typename T>
		void fillObjects(const size_t listSize, std::function<std::shared_ptr<T>()> factory);

	/*	template<typename T>
		bool validateSegmentInfo(const SegmentInfo<T>& infoSeg);

		bool isIndicesOut(const sf::Vector2i indices) const;*/
	private:
		std::unordered_map <std::string, std::shared_ptr<Objects>> listObjects;
		std::unordered_set<std::string> toBeRemovedObjects;
		std::unique_ptr<RTree> m_rTree;
		bool initialized;
		
	};
}