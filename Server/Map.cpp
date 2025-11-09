#include "Map.h"

#include "Rtree.h"
#include "BotServer.h"
#include "Constants.h"
#include "ThornServer.h"
#include "FoodServer.h"
#include "HeroServer.h"

#include "Generator.h"

#include <stdexcept>

using namespace sf;
using namespace std;

constexpr int listBotSize = 10;
constexpr int listFoodSize = 500;
constexpr int listThornSize = 10;

constexpr int MAX_ATTEMPS = 30;
constexpr int MAX_DEPTH = 6;

namespace Server
{

	Map::Map()
	{
		m_rTree = make_unique<RTree>();
		initialized = false;
	}

	void Map::moveObj(std::shared_ptr<Objects> obj)
	{
		m_rTree->update(obj);
	}

	std::vector<std::shared_ptr<Objects>> Map::getNearObjects(const sf::FloatRect& area) const
	{
		return m_rTree->queryIntersections(area);
	}

	void Map::add(std::shared_ptr<Objects> obj)
	{
		listObjects[obj->getID()] = obj;
	}

	std::shared_ptr<Objects> Map::findObj(const std::string& id)
	{
		auto it = listObjects.find(id);
		if (it != listObjects.end()) {
			return it->second;
		}
		return nullptr;
	}

	void Map::createSceneObjects()
	{
		if (!initialized)
		{
		    fillObjects<Food>(50, [this]() { return createObj<Food>(static_cast<IRegistrator*>(this)); });
			fillObjects<Thorn>(1, [this]() { return createObj<Thorn>(static_cast<IRegistrator*>(this)); });
			fillObjects<Bot>(5, [this]() { return createObj<Bot>(static_cast<IRegistrator*>(this)); });
			initialized = true;
		}
	}


	std::pair<sf::Vector2f, bool> Map::findFreePositionInQuad(const sf::FloatRect& quad, float radius, int currentDepth)
	{
		if (currentDepth >= MAX_DEPTH) return { Vector2f(), false };
		if (quad.width < 2 * radius || quad.height < 2 * radius) return { Vector2f(), false };

		if (isQuadFree(quad, radius))
		{
			Vector2f center =
			{
				genNumber<float>(quad.left + radius, quad.left + quad.width - radius),
				genNumber<float>(quad.top + radius, quad.top + quad.height - radius)
			};

			return { center, true };
		}

		float midX = quad.left + quad.width / 2.f;
		float midY = quad.top + quad.height / 2.f;

		FloatRect children[4] =
		{
			{quad.left, quad.top, midX - quad.left, midY - quad.top},
			{midX, quad.top, quad.left + quad.width - midX, midY - quad.top},
			{quad.left, midY, midX - quad.left, quad.top + quad.height - midY},
			{midX, midY, quad.left + quad.width - midX, quad.top + quad.height - midY}
		};

		for (const auto& child : children)
		{
			auto result = findFreePositionInQuad(child, radius, currentDepth + 1);
			if (result.second) return result;
		}
		return { Vector2f(), false };
	}


	bool Map::isQuadFree(const sf::FloatRect& quad, const float radius)
	{
		auto intersectionObjects = m_rTree->queryIntersections(quad);

		for (const auto& obj : intersectionObjects)
		{
			Vector2f center = obj->getCenter();
			float closestX = max(quad.left, min(center.x, quad.left + quad.width));
			float closestY = max(quad.top, min(center.y, quad.top + quad.height));

			if (hypot(center.x - closestX, center.y - closestY) < radius + obj->getRadius())
			{
				return false;
			}
		}
		return true;
	}

	bool Map::spawn(float radius, Vector2f& spawnPosition)
	{
		float minX = radius;
		float maxX = MapConstants::mapWidth - radius;
		float minY = radius;
		float maxY = MapConstants::mapHeight - radius;

		for (int i = 0; i < MAX_ATTEMPS; ++i)
		{
			Vector2f randPoint(genNumber<float>(minX, maxX), genNumber<float>(minY, maxY));

			auto& neighbors = m_rTree->queryIntersections(FloatRect(randPoint.x - radius, randPoint.y - radius, radius * 2, radius * 2));

			bool isFound = true;
			for (auto& other : neighbors)
			{
				if (circleIntersectCircle(randPoint, radius, other->getCenter(), other->getRadius()))
				{
					isFound = false;
					break;
				}
			}

			if (isFound)
			{
				spawnPosition = randPoint;
				return true;
			}
		}
		return false;


		/*for (int i = 0; i < MAX_ATTEMPS; ++i)
		{
			auto pair = findFreePositionInQuad(FloatRect(0, 0, MapConstants::mapWidth, MapConstants::mapHeight), radius, 0);
			if (!pair.second) continue;

			spawnPosition = pair.first;
			return true;
		}
		return false;*/
	}

	void Map::registerAuxiliary(std::shared_ptr<Objects> obj)
	{
		m_rTree->insert(obj->getBounds(), obj);
	}

	void Map::unregisterAuxiliary(std::shared_ptr<Objects> obj)
	{
		m_rTree->remove(obj->getID(), obj->getBounds());
	}

	template<typename T, typename ...Args>
	std::shared_ptr<T> Map::createObj(Args... args)
	{
		return std::make_shared<T>(args...);
	}

	template<typename T>
	void Map::fillObjects(const size_t listSize, std::function<std::shared_ptr<T>()> factory)
	{
		for (int i = 0; i < listSize; ++i)
		{
			shared_ptr<T> obj = factory();
			obj->registerSelf();
			shared_ptr<Objects> baseObj = static_pointer_cast<Objects>(obj);
			listObjects[baseObj->getID()] = baseObj;
		}
	}

	/*template<typename T>
	void Map::addInList(const std::string& id, const std::shared_ptr<T>& item)
	{
		if (id.empty() || item == nullptr)
		{
			return;
		}
		auto& list = getList<T>();
		list[id] = item;
	}

	template<>
	const std::unordered_map <std::string, std::shared_ptr<Thorn>>& Map::getList<Thorn>() const
	{
		return listThorns;
	}

	template<>
	std::unordered_map <std::string, std::shared_ptr<Thorn>>& Map::getList<Thorn>()
	{
		return listThorns;
	}


	template<>
	const std::unordered_map <std::string, std::shared_ptr<Food>>& Map::getList<Food>() const
	{
		return listFoods;
	}

	template<>
	std::unordered_map <std::string, std::shared_ptr<Food>>& Map::getList<Food>()
	{
		return listFoods;
	}

	template<>
	const std::unordered_map <std::string, std::shared_ptr<Hero>>& Map::getList<Hero>() const
	{
		return listHeroes;
	}

	template<>
	std::unordered_map <std::string, std::shared_ptr<Hero>>& Map::getList<Hero>()
	{
		return listHeroes;
	}

	template<>
	const std::unordered_map <std::string, std::shared_ptr<Bot>>& Map::getList<Bot>() const
	{
		return listBots;
	}

	template<>
	std::unordered_map <std::string, std::shared_ptr<Bot>>& Map::getList<Bot>()
	{
		return listBots;
	}

	template<>
	const std::unordered_map <std::string, std::shared_ptr<Feed>>& Map::getList<Feed>() const
	{
		return listFeeds;
	}

	template<>
	std::unordered_map <std::string, std::shared_ptr<Feed>>& Map::getList<Feed>()
	{
		return listFeeds;
	}*/


	//Vector2i Map::findSegmentIndices(Vector2f pos)
	//{
	//	int indexX = static_cast<int>(pos.x / segmentSize.x);
	//	int indexY = static_cast<int>(pos.y / segmentSize.y);
	//	return { indexX, indexY };
	//}

	//vector<Vector2i> Map::objectInSegments(Vector2f center, float radius)
	//{
	//	vector<Vector2i> res;

	//	Vector2f topLeft(center.x - radius, center.y - radius);
	//	Vector2f topRight(center.x + radius, center.y + radius);

	//	Vector2i start = findSegmentIndices(topLeft);
	//	Vector2i end = findSegmentIndices(topRight);


	//	start.x = clamp(start.x, 0, segmentCol - 1);
	//	start.y = clamp(start.y, 0, segmentRows - 1);
	//	end.x = clamp(end.x, 0, segmentCol - 1);
	//	end.y = clamp(end.y, 0, segmentRows - 1);

	//	for (int row = start.y; row <= end.y; ++row)
	//	{
	//		for (int col = start.x; col <= end.x; ++col)
	//		{
	//			float segX = col * segmentSize.x;
	//			float segY = row * segmentSize.y;

	//			float closestX = clamp(center.x, segX, segX + segmentSize.x);
	//			float closestY = clamp(center.y, segY, segY + segmentSize.y);

	//			float deltaX = center.x - closestX;
	//			float deltaY = center.y - closestY;

	//			if ((pow(deltaX, 2) + pow(deltaY, 2)) <= pow(radius, 2))
	//			{
	//				res.push_back({ col, row });
	//			}
	//		}
	//	}
	//	return res;
	//}

	//segmentInterval Map::getSegmentInterval(sf::Vector2f center, sf::Vector2f sizeParamObj) const
	//{
	//	Vector2f leftUpPos = { center - sizeParamObj * 0.5f };
	//	

	//	auto [leftBounder, rightBounder] = calcBoundsCyclic(leftUpPos.x, sizeParamObj.x, segmentSize.x, segmentCol);
	//	auto [topBounder, bottomBounder] = calcBoundsCyclic(leftUpPos.y, sizeParamObj.y, segmentSize.y, segmentRows);
	//	

	//	return
	//	{
	//		static_cast<short>(leftBounder),
	//		static_cast<short>(rightBounder),
	//		static_cast<short>(topBounder),
	//		static_cast<short>(bottomBounder)
	//	};
	//}

	//void Map::addInRespawnList(const int timerForResp, std::shared_ptr<Objects> obj)
	//{
	//	respawnList.push_back({ timerForResp, obj });
	//}

	void Map::TimeElapsed(int diff)
	{
		for (auto& obj : listObjects)
		{
			obj.second->TimeElapsed(diff);
		}

		for (auto& eatObj: listObjects)
		{
			auto nearObjects = m_rTree->queryIntersections(eatObj.second->getBounds());
			for (auto& nearObj: nearObjects)
			{
				if (&eatObj.second == &nearObj)
				{
					continue;
				}
				if (eatObj.second->Eat(*nearObj) && nearObj->getState() == ObjectState::Delitable)
				{
					toBeRemovedObjects.insert(nearObj->getID());
				}
			}
		}

		for (auto& id : toBeRemovedObjects)
		{
			listObjects.erase(id);
		}

	}

	//void Map::removeInList(const std::string& id, ObjectType type)
	//{
	//	switch (type)
	//	{
	//	case ObjectType::FOOD:
	//		listFoods.erase(id);
	//		break;
	//	case ObjectType::THORN:
	//		listThorns.erase(id);
	//		break;
	//	case ObjectType::BOT:
	//		listBots.erase(id);
	//		break;
	//	case ObjectType::PLAYER:
	//		listHeroes.erase(id);
	//		break;
	//	case ObjectType::FEED:
	//		listFeeds.erase(id);
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//bool Map::isInterval(const short firstInd, const short secondInd, const segmentInterval& interval, const Vector2f& sizeParamObj)
	//{
	//	bool gapX = interval.left >= interval.right && sizeParamObj.x > segmentSize.x;
	//	bool gapY = interval.top >= interval.bottom && sizeParamObj.y > segmentSize.y;

	//	bool isX = gapX ? (firstInd >= interval.left || firstInd <= interval.right)
	//		: (firstInd >= interval.left && firstInd <= interval.right);

	//	bool isY = gapY ? (secondInd >= interval.top || secondInd <= interval.bottom)
	//		: (secondInd >= interval.top && secondInd <= interval.bottom);
	//	return isX && isY;
	//}

	//std::pair<int, int> Map::calcBoundsCyclic(float pos, float sizeParamObj, float segmentSize, int segCount) const
	//{
	//	int left = static_cast<int>(floor(pos / segmentSize));
	//	int right = static_cast<int>(floor((pos + sizeParamObj) / segmentSize));

	//	left = normalize(left, segCount);
	//	right = normalize(right, segCount);

	//	return { left, right };
	//}

	//int Map::normalize(int ind, int segCount) const
	//{
	//	int res = ind % segCount;
	//	return (res < 0) ? res + segCount : res;
	//}


	//const MapSegment& Map::getSegment(sf::Vector2i indices) const
	//{
	//	if (isIndicesOut(indices))
	//	{
	//		cerr << "indices out of Segment\n";
	//		throw std::out_of_range("indices out of Segment");
	//	}
	//	return m_mapSegments[indices.x][indices.y];
	//}

	//MapSegment& Map::getSegment(sf::Vector2i indices)
	//{
	//	if (isIndicesOut(indices))
	//	{
	//		cerr << "indices out of Segment\n";
	//		throw std::out_of_range("indices out of Segment");
	//	}
	//	return m_mapSegments[indices.x][indices.y];
	//}

	//bool Map::isIndicesOut(const sf::Vector2i indices) const
	//{
	//	return (indices.x < 0 || indices.y < 0 || indices.x >= segmentCol || indices.y >= segmentRows);
	//}

	/*template void Map::addInList<Food>(const std::string& id, const std::shared_ptr<Food>& item);
	template void Map::addInList<Thorn>(const std::string& id, const std::shared_ptr<Thorn>& item);
	template void Map::addInList<Bot>(const std::string& id, const std::shared_ptr<Bot>& item);
	template void Map::addInList<Hero>(const std::string& id, const std::shared_ptr<Hero>& item);
	template void Map::addInList<Feed>(const std::string& id, const std::shared_ptr<Feed>& item);*/

	template void Map::fillObjects<Food>(const size_t, std::function<std::shared_ptr<Food>()>);

	template void Map::fillObjects<Bot>(const size_t, std::function<std::shared_ptr<Bot>()>);

	template void Map::fillObjects<Thorn>(const size_t, std::function<std::shared_ptr<Thorn>()>);
}