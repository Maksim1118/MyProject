#pragma once
#include "FoodServer.h"
#include "HeroServer.h"
#include "ThornServer.h"
#include "nlohmann\json.hpp"
#include "Map.h"
#include "QuadTree.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>


#define PI 3.141592653589793


namespace  Server
{
	constexpr auto ArrFoodSize = 500;
	constexpr int SetPosFood = 1190;
	constexpr int SetPosThorn = 1100;

	constexpr float timeRespawnFood = 200.f;
	constexpr float timeRespawnThorn = 3000.f;
	constexpr float timeRespawnBot = 10000.f;

	constexpr float offsetSpawn = 2.f;

	constexpr float EPSILON = 0.001f;

	constexpr int maxAttempsSpawn = 100;
	constexpr float densePercent = 0.4f;

	class SceneManager;
	class GameEngine
	{
	public:
		GameEngine();
		nlohmann::json process(nlohmann::json request);
		/*bool isCollWithMap(const Vector2f& pos);*/
		/*Vector2f getCoorCollWithMap(const  Vector2f& pos);*/
		void addInRemoveList(const std::string& id);

		Vector2f getCoordinateFromMap(const Vector2f& center);
		Vector2f getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height);
		virtual void TimeElapsed(int& diff);

		bool thornEaten = false;

	/*	std::unordered_map <std::string, std::shared_ptr<Thorn>> listThorns;
		std::unordered_map <std::string, std::shared_ptr<Food>> listFoods;
		std::unordered_map <std::string, std::shared_ptr<Hero>> listHeroes;
		std::unordered_map <std::string, std::shared_ptr<Bot>> listBots;
		std::unordered_map <std::string, std::shared_ptr<Feed>> m_ListFeeds;*/

	private:
		std::unordered_set <std::string> respawnList;
		std::unordered_set <std::string> removeList;

		Map m_map;
		Camera m_camera;

		void updateMoveObjectFromMapBounder(shared_ptr<MoveObject> obj);
		std::vector<sf::FloatRect> computeFreeZones(const sf::FloatRect& mapBounds, const std::vector<sf::FloatRect>& occupiedZones);
		std::vector<sf::FloatRect> subtractRect(const sf::FloatRect& freeZone, const sf::FloatRect& occ);
		
		template<typename T>
		bool spawnObj(std::shared_ptr<T>& obj);
		template<typename T>
		void fillObjects(size_t listSize);
		/*bool areAdjacentOrOverlapping(const sf::FloatRect& f1, const sf::FloatRect& f2);
		sf::FloatRect boundingRect(const sf::FloatRect& f1, const sf::FloatRect& f2);
		std::vector<sf::FloatRect> mergeFreeZones(const std::vector<sf::FloatRect> & zones);*/
		QuadTree quadTree;
		/*template<typename T>
		void updateSegments(const std::string& id, const std::shared_ptr<T>& item)
		{

			if (!item)
			{
				std::cerr << "error: memory read\n";
				return;
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
						removeFromSegment(prevInfo.indices, id, item);
						addToSegment(indices, id, item);
					}
				}
			}
			else
			{
				addToSegment(indices, id, item);
			}
			
		}*/


		list<weak_ptr<Objects>> m_WeakListPtr;
		static float m_TimeUpdateListObjects;
		static nlohmann::json HeroToJson(Hero& hero);
		static nlohmann::json BotToJson(Bot& bot);
		static nlohmann::json PieceToJson(Piece& piece);
		static nlohmann::json FoodToJson(Food& food);
		static nlohmann::json FeedToJson(Feed& feed);
		static nlohmann::json ThornToJson(Thorn& thorn);
		static nlohmann::json colorToJson(sf::Color color);

		template<typename Container, typename Func, typename PtrGetter>
		static void fillListToJson(Container& list, Func convertFunc, PtrGetter getPtr, nlohmann::json& outJsonArray)
		{
			/*cout <<"\n\nlist size: "<<  list.size() << endl;*/
			for (auto& element : list)
			{
				auto* objPtr = getPtr(element);
				if (!objPtr || !objPtr->isLive())
				{
					continue;
				}
					

				auto jsonObj = convertFunc(*objPtr);
	
				outJsonArray.push_back(jsonObj);
			}
			/*cout << "jsonArraysize: " << outJsonArray.size() << endl << endl;*/
		}

		bool objInSegment(shared_ptr<Objects>& obj, int segFirstInd, int segSecondInd,sf::Vector2f shift = { 0, 0 });

		void updatePlayersList(float diff);
		void updateFeedsList(float diff);



		/*void allObjectsCollWithMap();*/
		void calcDirMoveBot(int diff);
		Vector2f calcDistBotAndObj(Objects& obj1, Objects& obj2, float zeroZone);
		bool checkAllCollision(Objects& obj);

		template<typename T, typename Obj>
		void updateAndRemove(std::list<std::shared_ptr<T>>& objectList, std::list<std::weak_ptr<Obj>>& weakList, float diff) {
			for (auto it = objectList.begin(); it != objectList.end();) {
				(*it)->TimeElapsed(diff);
				if ((*it)->state == States::EATEN)
				{
					/*shared_ptr<T> tempPtr = *it;
					weakList.push_back(tempPtr);*/
					it = objectList.erase(it);
				}
				else {
					++it;
				}
			}
		}

		/*template<typename T> void updateList(std::unordered_map <std::string, std::shared_ptr<T>>& list, float diff)
		{
			for (auto& obj : list)
			{
				obj.second->TimeElapsed(diff);
			}
		}*/

		void clearWeakList();

		template<typename T> void spawnList(list<shared_ptr<T>>& _list)
		{
			for (auto& i = _list.begin(); i != _list.end(); i++)
			{
				shift((*i).get());
				while (checkAllCollision(*i->get()))
				{
					shift((*i).get());
				}
			}
		}

		template<typename T> void shift(T* ptrObj)
		{
			float x = (float)(rand() % fieldWidth);
			float y = (float)(rand() % fieldHeight);
			ptrObj->setCenter(x, y);
		}

	/*	void shiftObj(shared_ptr<Objects> ptrObj)
		{
			float x = (float)(rand() % fieldWidth);
			float y = (float)(rand() % fieldHeight);
			ptrObj->setCenter(x, y);
		}*/

		/*void fillList(unordered_map<int, shared_ptr<Objects>>& list, size_t size)
		{
			if (size > 0)
			{
				for (int i = 0; i < size; i++)
				{
					shared_ptr<T> obj;
					obj = make_shared<T>();
					if (obj)
					{
						list.push_back(obj);
					}
				}
			}
			
		}*/

		

		/*template<typename T, typename STsegments, typename SpawnFunc, typename FindSegIndFunc>
		void fillSegments(int listSize, STsegments& segments,SpawnFunc spawnFunc, FindSegIndFunc findSegIndFunc)
		{
			for (int i = 0; i < listSize; ++i)
			{
				std::shared_ptr<T> obj = std::make_shared<T>();

				sf::Vector2f centerObj = spawnFunc(obj);
				std::vector<sf::Vector2i> segmentIndices = findSegIndFunc(obj);
				if (segmentIndices.empty())
				{
					obj = nullptr;
					continue;
				}
				for (int j = 0; j < segmentIndices.size(); ++i)
				{
					segments[segmentIndices[j].x][segmentIndices[j].y].getSegmentList<T>()[obj->getID()] = obj;
				}
			}
		}*/

		template<typename T, typename STsegments, typename SpawnFunc, typename FindSegIndFunc>
		void fillSegments(int listSize, STsegments& segments, SpawnFunc spawnFunc, FindSegIndFunc findSegIndFunc)
		{
			for (int i = 0; i < listSize; ++i)
			{
				std::shared_ptr<T> obj = std::make_shared<T>();

				sf::Vector2f centerObj = spawnFunc(obj);
				std::vector<sf::Vector2i> segmentIndices = findSegIndFunc(obj);
				if (segmentIndices.empty())
				{
					obj = nullptr;
					continue;
				}
				for (int j = 0; j < segmentIndices.size(); ++j)
				{
					segments[segmentIndices[j].x][segmentIndices[j].y].getSegmentList<T>()[obj->getID()] = obj;
				}
			}
		}

	/*	template<typename T>
		void updateIndices(std::shared_ptr<T> obj)
		{
			obj->m_curSegIndices = findSegmentIndecies(obj);
		}*/

		/*template<typename T>
		void addIndices(std::vector<sf::Vector2i>& indices, std::shared_ptr<T> obj)
		{
			for (auto& ind : indices)
			{
				m_segments[ind.x][ind.y].getSegmentList<T>()[obj->getID()] = obj;
			}
		}

		template<typename T>
		void fillSegments(std::unordered_map<std::string, std::shared_ptr<T>>& container)
		{
			for (auto& [key, obj] : container)
			{
				updateIndices(obj);
				addIndices(obj->m_curSegIndices, obj);
			}
		}*/

		/*sf::Vector2i findSegmentIndices(sf::Vector2f pos);*/

		/*std::vector<sf::Vector2i> objectInSegments(sf::Vector2f center, float radius);*/

		/*template<typename T>
		void updateSegments(float diff, std::unordered_map<std::string, std::shared_ptr<T>>& mainList)
		{
			for (const auto& [key, item] : mainList)
			{
				item->TimeElapsed(diff);
				Vector2i indices = findSegmentIndecies(item->getCenter());
				if (isOutSegment(indices))
				{
					continue;
				}

				if (item->m_currSegIndices.x < 0 || item->m_currSegIndices.x >= segmentCol
					|| item->m_currSegIndices.y < 0 || item->m_currSegIndices.y >= segmentRows)
				{
					m_mapSegments[indices.x][indices.y].getSegmentList<T>()[key] = item;
					item->m_currSegIndices = indices;
				}
				else if (indices != item->m_currSegIndices)
				{
					m_mapSegments[item->m_currSegIndices.x][item->m_currSegIndices.y].getSegmentList<T>().erase(key);
					m_mapSegments[indices.x][indices.y].getSegmentList<T>()[key] = item;
					item->m_currSegIndices = indices;
				}
			}
		}*/
		bool isOutSegment(sf::Vector2i indices);

		/*template<typename T>
		void addToSegment(const sf::Vector2i& indices, const std::string& id, const std::shared_ptr<T>& item)
		{
			m_mapSegments[indices.x][indices.y].getSegmentList<T>()[id] = item;
		}*/

		/*template<typename T>
		bool findObjInSegments(const std::string& id, SegmentInfo<T>& InfoOut)
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
		}*/

		/*template<typename T>
		void removeFromSegment(const sf::Vector2i& indices, const std::string& id, const std::shared_ptr<T>& item)
		{
			m_mapSegments[indices.x][indices.y].getSegmentList<T>().erase(id);
		}*/

		/*template<typename T>
		bool validateSegmentInfo(const SegmentInfo<T>& infoSeg)
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
		}*/


		/*template <typename T>
		void spawn(std::shared_ptr<T> obj)
		{
			shift(obj.get());
			while (checkAllCollision(*obj))
			{
				shift(obj.get());
			}
		}*/

		template<typename T> void updatePosAfterEaten(shared_ptr<T>& ptrObj, list<shared_ptr<T>>& listPtrObjects)
		{
			bool collision = true;
			while (collision)
			{
				shift(ptrObj.get());
				collision = checkAllCollision(*ptrObj.get());
			}
		}

		template<typename T> bool checkCollision(Objects* obj, std::unordered_map<std::string, shared_ptr<T>>& list)
		{
			for (auto& [key, item] : list)
			{
				if (obj != item.get() && obj->isCollision(*item, 2))
				{
					return true;
				}
			}
			return false;
		}

		


		template<typename T>
		void updateAndRespawn(std::list<std::shared_ptr<T>>& objectList, std::list<float>& respawnTimeList, float TimeRespawn, float diff) {
			
			for(auto it = objectList.begin(); it != objectList.end();)
			{
				if ((*it)->state == States::EATEN) {
					respawnTimeList.push_back(TimeRespawn);
					
					it = objectList.erase(it);
				}
				else {
					++it;
				}
			}

			for (auto & it = respawnTimeList.begin(); it != respawnTimeList.end();)
			{
				(*it) -= diff;
				if ((*it) <= 0)
				{
					shared_ptr<T> obj = make_shared<T>();
					objectList.push_back(obj);
					it = respawnTimeList.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	};
	template<typename T>
	inline void GameEngine::fillObjects(size_t listSize)
	{
		auto& list = m_map.getGlobalList<T>();
		for (int i = 0; i < listSize; i++)
		{
			std::shared_ptr<T> obj;
			obj = std::make_shared<T>();
			if (obj)
			{
				if (!spawnObj<T>(obj))
				{
					cerr << "error spawn\n";
				}
				else
				{
					if (!m_map.addToGlobalList(obj->getID(), obj))
					{
						cerr << "error add To Global List in Fill Func\n";
					}
				}
			}
		}
	}
	template<typename T>
	inline bool GameEngine::spawnObj(shared_ptr<T>& obj)
	{
		float rMin = obj->getRadius() + offsetSpawn;
		float rMax = obj->getRadius() * 4;
		Vector2f origin;
		origin.x = genNumber<float>(rMax, fieldWidth - rMax );
		origin.y = genNumber<float>(rMax, fieldHeight - rMax);

		for (int attempt = 0; attempt < maxAttempsSpawn; ++attempt)
		{
			float dist = genNumber<float>(rMin, rMax);
			const int angleSteps = 20;
			for (int angleIdx = 0; angleIdx < angleSteps; ++angleIdx)
			{
				float angle = 2.f * PI * angleIdx / angleSteps;
				Vector2f newPoint = origin + Vector2f(std::cos(angle), std::sin(angle)) * dist;

				FloatRect queryRect(newPoint.x - rMin, newPoint.y - rMin, rMin * 2.f, rMin * 2.f);
				const auto& candidates = quadTree.retrive(queryRect);

				bool collision = false;
				for (const auto& candidate : candidates)
				{
					Vector2f candidateCenter(candidate.bounds.left + candidate.bounds.width / 2.f, candidate.bounds.top + candidate.bounds.height / 2.f);
					float candidateRadius = std::min(candidate.bounds.width, candidate.bounds.height) / 2.f;

					if (circleIntersectCircle(candidateCenter, candidateRadius, newPoint, rMin))
					{
						collision = true;
						break;
					}
				}

				if (!collision)
				{
					obj->setCenter(newPoint);
					ObjectData data;
					data.bounds = obj->getBounds();
					data.id = obj->getID();
					data.center = obj->getCenter();
					data.radius = obj->getRadius();
					quadTree.insert(data);
					return true;
				}
			}
		}
		return false;
		//float diametr = obj->getRadius() * 2;
		//vector<FloatRect> occZones = quadTree.getAllBounds();

		//vector<FloatRect> freeZones = computeFreeZones(quadTree.getBounds(),occZones);
		///*for (const auto& zone : freeZones)
		//{
		//	std::cout << "Zone: (" << zone.left << ", " << zone.top << "), w=" << zone.width << ", h=" << zone.height << "\n";
		//}*/
		//
		//freeZones.erase(remove_if(freeZones.begin(), freeZones.end(),
		//	[diametr](const FloatRect& zone)
		//	{
		//		return zone.width < diametr + 2 * offsetSpawn || zone.height < diametr + 2 * offsetSpawn;;
		//	}), freeZones.end());

		//if (freeZones.empty())
		//{
		//	return false;
		//}

		//const int maxAttempsPerZone = 50;
		//const int maxAttemps = 500;
		//int attemps = 0;

		//vector<int> zoneIndices(freeZones.size());
		//iota(zoneIndices.begin(), zoneIndices.end(), 0);
		//shuffle(zoneIndices.begin(), zoneIndices.end(), gen);

		//for (int zoneInd : zoneIndices)
		//{
		//	const FloatRect& zone = freeZones[zoneInd];

		//	float left = zone.left + obj->getRadius() + offsetSpawn;
		//	float right = zone.left + zone.width  - obj->getRadius() - offsetSpawn ;
		//	float top = zone.top + obj->getRadius() + offsetSpawn;
		//	float bottom = zone.top + zone.height - obj->getRadius() - offsetSpawn;

		//	
		//	for (int attemp = 0; attemp < maxAttempsPerZone && attemps < maxAttemps; ++attemp, ++attemps)
		//	{
		//		float centerX = genNumber(left, right);
		//		float centerY = genNumber(top, bottom);

		//		bool isIntersect = quadTree.isIntersect({ centerX, centerY }, obj->getRadius());
		//		if (!isIntersect)
		//		{
		//			obj->setCenter(centerX, centerY);
		//			ObjectData data;
		//			data.bounds = obj->getBounds();
		//			data.center = obj->getCenter();
		//			data.id = obj->getID();
		//			data.radius = obj->getRadius();
		//			quadTree.insert(data);
		//			return true;
		//		}
		//
		//	}
		//}
		//cout << "false\n";
		//return false;
	}
	void runServer(GameEngine& _Engine);
}

