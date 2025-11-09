#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>


namespace Server
{

	/*struct ObjectQuad
	{
		std::weak_ptr<Objects> obj;
		sf::FloatRect bounds;
		ObjectQuad(const std::shared_ptr<Objects> obj, const sf::FloatRect& bounds);
	};*/
	//template<typename T>
	//struct QuadTreeEntry
	//{
	//	std::shared_ptr<T> obj;
	//	QuadTree<T>* currentNode = nullptr;

	//	void reset()
	//	{
	//		obj.reset();
	//		currentNode = nullptr;
	//	}
	//};

	//template<typename T>
	//class Pool
	//{
	//public:
	//	Pool(size_t initialSize)
	//	{
	//		pool.reserve(initialSize);
	//		available.reserve(initialSize);
	//		for (size_t i = 0; i < initialSize; ++i)
	//		{
	//			pool.push_back(std::make_shared<T>());
	//			available.push_back(pool.back().get());
	//		}
	//	}

	//	T* acquaire()
	//	{
	//		if (available.empty())
	//		{
	//			pool.push_back(std::make_shared<T>());
	//			return pool.back().get();
	//		}

	//		T* obj = available.back();
	//		available.pop_back();
	//		return obj;
	//	}

	//	void release(T* obj)
	//	{
	//		if (obj)
	//		{
	//			resetObj(obj);
	//			available.push_back(obj);
	//		}
	//	}

	//	size_t poolSize() const { return pool.size(); }

	//private:
	//	void resetObj(T* obj)
	//	{
	//		if constexpr (std::is_member_function_pointer_v<decltype(&T::reset)>)
	//		{
	//			obj->reset();
	//		}
	//	}

	//	std::vector<std::unique_ptr<T>> pool;
	//	std::vector<T*> available;
	//};

	//using Entity = std::string;

	//template<typename T>
	//class QuadTree
	//{
	//public:

	//	void insert(std::shared_ptr<T> obj);
	//	bool remove(const std::string& id);
	//	void clear();
	//	
	//	int getIndex(const sf::FloatRect& rect) const;
	//	void split();
	//	sf::FloatRect getLeastDenseQuadrant() const;
	//	/*void tryEat(std::shared_ptr<MoveObject> obj);
	//	void updateInTreePos(std::shared_ptr<MoveObject> obj);*/

	//	QuadTree(int pLevel, sf::FloatRect pBounds, size_t poolSize);
	//	~QuadTree();
	//private:
	//	static const int MAX_OBJECTS = 4;
	//	static const int MAX_LEVELS = 5;

	//	int level;
	//	int objectCount;

	//	sf::FloatRect bounds;

	//	std::unordered_map<Entity, sf::FloatRect> positions;
	//	std::unordered_map<Entity, ObjectType> types;

	//	std::unique_ptr<QuadTree> nodes[4];

	//};

	//template<typename T>
	//QuadTree<T>::QuadTree(int pLevel, sf::FloatRect pBounds, size_t poolSize)
	//	: level(pLevel), bounds(pBounds), objectCount(0), entryPool(poolSize)
	//{
	//	for (int i = 0; i < 4; ++i)
	//		nodes[i] = nullptr;
	//}

	//template<typename T>
	//QuadTree<T>::~QuadTree()
	//{
	//	clear();
	//}

	//template<typename T>
	//void QuadTree<T>::insert(std::shared_ptr<T> obj)
	//{
	//	if (!obj) return;

	//	if (nodes[0])
	//	{
	//		int index = getIndex(obj.bounds);
	//		if (index != -1)
	//		{
	//			nodes[index]->insert(obj);
	//			++objectCount;
	//			return;
	//		}
	//	}

	//	auto entry = entryPool.acquaire();
	//	entry->obj = obj;
	//	enty->currentNode = this;

	//	objects[obj->getID()] = obj;
	//	++objectCount;

	//	if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
	//	{
	//		if (!nodes[0])
	//		{
	//			split();
	//		}

	//		for (auto it = objects.begin(); it != objects.end();)
	//		{
	//			int index = getIndex(it->second.bounds);
	//			if (index != -1)
	//			{
	//				nodes[index]->insert(it->second);
	//				--objectCount;
	//				it = objects.erase(it);
	//			}
	//			else
	//			{
	//				++it;
	//			}
	//		}
	//	}
	//}

	//template<typename T>
	//bool QuadTree<T>::remove(const std::string& id)
	//{
	//	auto it = objects.find(id);

	//	if (it != objects.end())
	//	{
	//		objects.erase(it);
	//		--objectCount;
	//		return true;
	//	}

	//	bool removed = false;
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		if (nodes[i] && nodes[i]->remove(id))
	//		{
	//			--objectCount;
	//			removed = true;
	//		}
	//	}
	//	return removed;
	//}

	//template<typename T>
	//void QuadTree<T>::clear()
	//{
	//	objects.clear();
	//	objectCount = 0;
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		if (node)
	//		{
	//			node[i]->clear();
	//			node[i].reset();
	//		}
	//	}
	//}

	//template<typename T>
	//int QuadTree<T>::getIndex(const FloatRect& rect) const
	//{
	//	int index = -1;
	//	float verticalMidPoint = bounds.left + bounds.width / 2;
	//	float horizontalMidPoint = bounds.top + bounds.height / 2;

	//	bool topQuadrant = (rect.top < horizontalMidPoint
	//		&& rect.top + rect.height <= horizontalMidPoint);
	//	bool bottomQuadrant = (rect.top >= horizontalMidPoint);

	//	if ((rect.left < verticalMidPoint) && (rect.left + rect.width <= verticalMidPoint))
	//	{
	//		if (topQuadrant)
	//		{
	//			index = 1;
	//		}
	//		else if (bottomQuadrant)
	//		{
	//			index = 2;
	//		}
	//	}
	//	else if (rect.left >= verticalMidPoint)
	//	{
	//		if (topQuadrant)
	//		{
	//			index = 0;
	//		}
	//		else if (bottomQuadrant)
	//		{
	//			index = 3;
	//		}
	//	}

	//	return index;
	//}

	//template<typename T>
	//void QuadTree<T>::split()
	//{
	//	float halfW = bounds.width / 2.f;
	//	float halfH = bounds.height / 2.f;
	//	float x = bounds.left;
	//	float y = bounds.top;

	//	nodes[0] = make_unique<QuadTree>(level + 1, FloatRect(x + halfW, y, halfW, halfH));
	//	nodes[1] = make_unique<QuadTree>(level + 1, FloatRect(x, y, halfW, halfH));
	//	nodes[2] = make_unique<QuadTree>(level + 1, FloatRect(x, y + halfH, halfW, halfH));
	//	nodes[3] = make_unique<QuadTree>(level + 1, FloatRect(x + halfW, y + halfH, halfW, halfH));
	//}

	//template<typename T>
	//inline sf::FloatRect QuadTree<T>::getLeastDenseQuadrant() const
	//{
	//	if (!nodes[0])
	//	{
	//		return bounds;
	//	}

	//	int minIndex = 0;
	//	int minCount = nodes[0]->objectCount;
	//	
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		if (node[i]->objectCount < minCount)
	//		{
	//			minCount = node[i]->objectCount;
	//			minIndex = i;
	//		}
	//	}
	//	return nodes[minIndex]->bounds;
	//}

}