#include "QuadTree.h"


#include "BaseOld.h"
namespace Server
{

	using namespace std;
	using namespace sf;

	QuadTree::QuadTree(int pLevel, FloatRect pBounds)
		:level(pLevel), bounds(pBounds)
	{
	}

	int QuadTree::getIndex(const FloatRect rect) const
	{
		int index = -1;
		float verticalMidPoint = bounds.left + bounds.width / 2;
		float horizontalMidPoint = bounds.top + bounds.height / 2;
		
		bool topQuadrant = (rect.top < horizontalMidPoint
			&& rect.top + rect.height < horizontalMidPoint);
		bool bottomQuadrant = (rect.top > horizontalMidPoint);

		if (rect.left < verticalMidPoint && rect.left + rect.width < verticalMidPoint)
		{
			if (topQuadrant)
			{
				index = 1;
			}
			else if (bottomQuadrant)
			{
				index = 2;
			}
		}
		else if (rect.left > verticalMidPoint)
		{
			if (topQuadrant)
			{
				index = 0;
			}
			else if (bottomQuadrant)
			{
				index = 3;
			}
		}

		return index;
	}

	void QuadTree::split()
	{
		float halfW = bounds.width / 2.f;
		float halfH = bounds.height / 2.f;

		nodes[0] = make_unique<QuadTree>(level + 1, FloatRect(bounds.left + halfW, bounds.top, halfW, halfH));
		nodes[1] = make_unique<QuadTree>(level + 1, FloatRect(bounds.left, bounds.top, halfW, halfH));
		nodes[2] = make_unique<QuadTree>(level + 1, FloatRect(bounds.left, bounds.top + halfH, halfW, halfH));
		nodes[3] = make_unique<QuadTree>(level + 1, FloatRect(bounds.left + halfW, bounds.top + halfH, halfW, halfH));
	}

	bool QuadTree::remove(const std::string id)
	{
		auto it = find_if(objects.begin(), objects.end(),
			[id](const ObjectData& data) {return data.id == id; });
		if (it != objects.end())
		{
			objects.erase(it);
			return true;
		}

		if (nodes[0])
		{
			for (auto& node : nodes)
			{
				if (node->remove(id))
					return true;
			}
		}
		return false;
	}

	vector<ObjectData> QuadTree::retrive(FloatRect& queryRect) const
	{
		vector<ObjectData> candidates;

		for (const auto obj : objects)
		{
			if (obj.bounds.intersects(queryRect))
			{
				candidates.push_back(obj);
			}
		}

		if (nodes[0])
		{
			int index = getIndex(queryRect);
			if (index != -1)
			{
				auto childCandidates = nodes[index]->retrive(queryRect);
				candidates.insert(candidates.end(), childCandidates.begin(), childCandidates.end());
			}
			else
			{
				for (const auto& node : nodes)
				{
					if (node && node->bounds.intersects(queryRect))
					{
						auto childCandidates = node->retrive(queryRect);
						candidates.insert(candidates.end(), childCandidates.begin(), childCandidates.end());
					}
				}
			}
		}
		return candidates;
	}

	/*sf::FloatRect QuadTree::getBounds() const
	{
		return bounds;
	}*/

	/*std::vector<sf::FloatRect> QuadTree::getAllBounds() const
	{
		vector<sf::FloatRect> allBounds;

		for (const auto& obj : objects)
		{
			allBounds.push_back(obj.bounds);
		}

		for (const auto& node : nodes)
		{
			if (node)
			{
				auto childBounds = node->getAllBounds();
				allBounds.insert(allBounds.end(), childBounds.begin(), childBounds.end());
			}
		}
		return allBounds;
	}*/

	/*bool QuadTree::isIntersect(const sf::Vector2f centerObj, const float radiusObj) const
	{
		FloatRect queryRect(centerObj.x - radiusObj, centerObj.y - radiusObj, radiusObj * 2.f, radiusObj * 2.f);

		if (!bounds.intersects(queryRect))
		{
			return false;
		}

		for (const auto& obj : objects)
		{
			if (circleIntersectCircle(centerObj, radiusObj, obj.center, obj.radius))
			{
				return true;
			}
		}

		if (nodes[0])
		{
			int index = getIndex(queryRect);

			if (index != -1)
			{
				if(nodes[index]->isIntersect(centerObj, radiusObj))
				{
					return true;
				}
			}
			else
			{
				for (const auto& node : nodes)
				{
					if (node && node->bounds.intersects(queryRect))
					{
						if (node->isIntersect(centerObj, radiusObj))
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}*/

	void QuadTree::insert(const ObjectData& obj)
	{
		if (nodes[0])
		{
			int index = getIndex(obj.bounds);
			if (index != -1)
			{
				nodes[index]->insert(obj);
				return;
			}
		}

		objects.push_back(obj);

		if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
		{
			if (!nodes[0])
			{
				split();
			}

			for (auto it = objects.begin(); it != objects.end();)
			{
				int index = getIndex(it->bounds);
				if (index != -1)
				{
					nodes[index]->insert(*it);
					it = objects.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
}
