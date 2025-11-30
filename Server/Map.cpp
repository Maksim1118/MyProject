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
		    fillObjects<Food>(100, [this]() { return createObj<Food>(static_cast<IRegistrator*>(this)); });
			fillObjects<Thorn>(1, [this]() { return createObj<Thorn>(static_cast<IRegistrator*>(this)); });
			fillObjects<Bot>(10, [this]() { return createObj<Bot>(static_cast<IRegistrator*>(this)); });
			m_rTree->print();
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
	}

	void Map::registerAuxiliary(std::shared_ptr<Objects> obj)
	{
		m_rTree->insert(obj->getMBR(), obj);
	}

	void Map::unregisterAuxiliary(std::shared_ptr<Objects> obj)
	{
		m_rTree->remove(obj->getID(), obj->getMBR());
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

	void Map::TimeElapsed(int diff)
	{
		for (auto& obj : listObjects)
		{
			obj.second->TimeElapsed(diff);
		}

		for (auto& eatObj: listObjects)
		{
			auto nearObjects = m_rTree->queryIntersections(eatObj.second->getMBR());
			for (auto& nearObj: nearObjects)
			{
				if (eatObj.second.get() == nearObj.get())
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

	template void Map::fillObjects<Food>(const size_t, std::function<std::shared_ptr<Food>()>);

	template void Map::fillObjects<Bot>(const size_t, std::function<std::shared_ptr<Bot>()>);

	template void Map::fillObjects<Thorn>(const size_t, std::function<std::shared_ptr<Thorn>()>);
}