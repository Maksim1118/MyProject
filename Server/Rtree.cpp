#include "Rtree.h"

#include "ObjectsServer.h"

#include "BaseOld.h"
#include "Constants.h"

#include <cassert>

using namespace std;
using namespace sf;

namespace Server
{
	constexpr int MAX_ENTRIES = 4;
	constexpr int MIN_ENTRIES = MAX_ENTRIES / 2;
	//RTree::RTreeNode::RTreeNode(bool isLeaf, std::shared_ptr<RTreeNode> _parent)
	//	:leaf(isLeaf), parent(_parent)
	//{
	//	mbr = FloatRect(0, 0, 0, 0);
	//}

	//void RTree::RTreeNode::recalcMBR()
	//{
	//	if (leaf)
	//	{
	//		if (entries.empty())
	//		{
	//			mbr = FloatRect(0, 0, 0, 0);
	//			return;
	//		}

	//		float left = numeric_limits<float>::max();
	//		float top = numeric_limits<float>::max();
	//		float right = numeric_limits<float>::lowest();
	//		float bottom = numeric_limits<float>::lowest();

	//		for (auto& e : entries)
	//		{
	//			FloatRect entryBounds = e.lock()->getBounds();
	//			left = min(left, entryBounds.left);
	//			top = min(top, entryBounds.top);
	//			right = max(right, entryBounds.left + entryBounds.width);
	//			bottom = max(bottom, entryBounds.top + entryBounds.height);
	//		}

	//		mbr = FloatRect(left, top, right - left, bottom - top);
	//	}
	//	else
	//	{
	//		if (children.empty())
	//		{
	//			mbr = FloatRect(0, 0, 0, 0);
	//			return;
	//		}
	//		float left = numeric_limits<float>::max();
	//		float top = numeric_limits<float>::max();
	//		float right = numeric_limits<float>::lowest();
	//		float bottom = numeric_limits<float>::lowest();

	//		for (auto& c : children)
	//		{
	//			c->recalcMBR();
	//			left = min(left, c->mbr.left);
	//			top = min(top, c->mbr.top);
	//			right = max(right, c->mbr.left + c->mbr.width);
	//			bottom = max(bottom, c->mbr.top + c->mbr.height);
	//		}

	//		mbr = FloatRect(left, top, right - left, bottom - top);
	//	}
	//}

	//RTree::RTree()
	//	:root(make_shared<RTreeNode>(true))
	//{

	//}


	//void RTree::insert(shared_ptr<Objects> entry)
	//{
	//	insertEntry(root, entry);
	//}

	//bool RTree::remove(const std::string& id)
	//{
	//	return removeEntry(root, id);
	//}

	//void RTree::update(std::shared_ptr<Objects> obj)
	//{
	//	if (!obj) return;

	//	shared_ptr<RTreeNode> leafNode;
	//	const string id = obj->getID();
	//	if (!findObjectAndNode(root, id, leafNode, obj))
	//	{
	//		return;
	//	}

	//	if (containsRect(leafNode->mbr, obj->getBounds()))
	//	{
	//		leafNode->recalcMBR();
	//		adjustTree(leafNode);
	//	}
	//	else
	//	{
	//		removeEntry(leafNode, id);
	//		insertEntry(root, obj);
	//	}
	//}

	///*bool RTree::update(const std::string& id, const sf::FloatRect& newBounds)
	//{
	//	ObjectType type;
	//	if (!findAndRemove(root.get(), id, type))
	//		return false;
	//	insert(id, newBounds, type);
	//	return true;
	//}*/

	//vector<shared_ptr<Objects>> RTree::queryIntersections(const FloatRect& queryRect) const
	//{
	//	vector<shared_ptr<Objects>> results;
	//	queryIntersectionsRecursive(root, queryRect, results);
	//	return results;
	//}

	//float RTree::area(const sf::FloatRect& r)
	//{
	//	return r.width * r.height;
	//}

	//sf::FloatRect RTree::combine(const sf::FloatRect& a, const sf::FloatRect& b)
	//{
	//	float left = min(a.left, b.left);
	//	float top = min(a.top, b.top);
	//	float right = max(a.left + a.width, b.left + b.width);
	//	float bottom = max(a.top + a.height, b.top + b.height);
	//	return { left, top, right - left, bottom - top };
	//}

	//float RTree::enlargement(const sf::FloatRect& mbr, const sf::FloatRect& r)
	//{
	//	return area(combine(mbr, r)) - area(mbr);
	//}


	//shared_ptr<RTree::RTreeNode> RTree::chooseLeaf(shared_ptr<RTreeNode> node, shared_ptr<Objects> entry)
	//{
	//	if (!node) return nullptr;

	//	if (node->leaf)
	//	{
	//		return node;
	//	}

	//	float minIncrease = numeric_limits<float>::max();
	//	float minArea = numeric_limits<float>::max();
	//	shared_ptr<RTreeNode> bestChild;
	//	
	//	for (auto& child : node->children)
	//	{
	//		float inc = enlargement(child->mbr, entry->getBounds());
	//		float areaChild = area(child->mbr);

	//		if (inc < minIncrease || (inc == minIncrease && areaChild < minArea))
	//		{
	//			minIncrease = inc;
	//			minArea = areaChild;
	//			bestChild = child;
	//		}
	//	}

	//	return chooseLeaf(bestChild, entry);
	//}

	//void RTree::insertEntry(shared_ptr<RTreeNode> node, shared_ptr<Objects> entry)
	//{
	//	shared_ptr<RTreeNode> leaf = chooseLeaf(node, entry);
	//	if (!leaf) return;

	//	leaf->entries.push_back(entry);

	//	if (leaf->entries.size() > RTreeNode::MAX_ENTRIES)
	//	{
	//		splitNode(leaf);
	//	}
	//	adjustTree(leaf);
	//}

	//void RTree::splitNode(shared_ptr<RTreeNode> node)
	//{
	//	if (!node) return; 

	//	if (node->leaf)
	//	{
	//		auto& entries = node->entries;
	//		size_t mid = entries.size() / 2;

	//		auto newNode = make_shared<RTreeNode>(true, node->parent.lock());

	//		vector<weak_ptr<Objects>> movedEntries(entries.begin() + mid, entries.end());
	//		entries.erase(entries.begin() + mid, entries.end());

	//		newNode->entries = movedEntries;
	//		newNode->recalcMBR();
	//		node->recalcMBR();

	//		if (!node->parent.lock())
	//		{
	//			auto newRoot = make_shared<RTreeNode>(false);
	//			node->parent = newRoot;
	//			newNode->parent = newRoot;

	//			newRoot->children.push_back(node);
	//			newRoot->children.push_back(newNode);
	//			root = newRoot;
	//		}
	//		else
	//		{
	//			newNode->parent = node->parent;
	//			node->parent.lock()->children.push_back(newNode);
	//		}
	//	}
	//	else
	//	{
	//		auto& children = node->children;
	//		size_t mid = children.size() / 2;

	//		auto newNode = make_shared<RTreeNode>(false, node->parent.lock());

	//		std::vector<std::shared_ptr<RTreeNode>> movedChildren;
	//		movedChildren.reserve(children.size() - mid);
	//		for (size_t i = mid; i < children.size(); ++i)
	//		{
	//			movedChildren.push_back(children[i]);
	//		}
	//		children.erase(children.begin() + mid, children.end());

	//		for (auto& child : movedChildren)
	//		{
	//			child->parent = newNode;
	//		}

	//		newNode->children = movedChildren;
	//		newNode->recalcMBR();
	//		node->recalcMBR();

	//		if (!node->parent.lock())
	//		{
	//			auto newRoot = make_shared<RTreeNode>(false);
	//			node->parent = newRoot;
	//			newNode->parent = newRoot;

	//			newRoot->children.push_back(root);
	//			newRoot->children.push_back(newNode);
	//			root = newRoot;
	//		}
	//		else
	//		{
	//			newNode->parent = node->parent;
	//			node->parent.lock()->children.push_back(newNode);
	//		}
	//	}
	//}

	//void RTree::adjustTree(shared_ptr<RTreeNode> node)
	//{
	//	if (!node) return;

	//	shared_ptr<RTreeNode> curNode = node;

	//	while (curNode)
	//	{ 
	//		curNode->recalcMBR();
	//		
	//		if (curNode->entries.size() > RTreeNode::MAX_ENTRIES)
	//		{
	//			splitNode(curNode);
	//		}
	//		else
	//		{
	//			if (curNode->children.size() > RTreeNode::MAX_ENTRIES)
	//			{
	//				splitNode(curNode);
	//			}
	//		}

	//		curNode = curNode->parent.lock();
	//	}
	//	if(root)
	//		root->recalcMBR();
	//}

	//void RTree::queryIntersectionsRecursive(shared_ptr<RTreeNode> node, const FloatRect& queryRect, vector<shared_ptr<Objects>>& results) const
	//{
	//	if (!node) return;

	//	if (node->leaf)
	//	{
	//		for (auto& entry : node->entries)
	//		{
	//			if (auto obj = entry.lock())
	//			{
	//				/*if( cyclicIntersectsRect(obj->getBounds(), queryRect,MapConstants::mapWidth, MapConstants::mapHeight))*/
	//				{
	//					results.push_back(obj);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		for (const auto& child : node->children)
	//		{
	//			/*if (cyclicIntersectsRect(child->mbr,queryRect, MapConstants::mapWidth, MapConstants::mapHeight))*/
	//			{
	//				queryIntersectionsRecursive(child, queryRect, results);
	//			}
	//		}
	//	}
	//}

	///*void RTree::tryEat(shared_ptr<MoveObject> eatObj, unordered_map<string, ObjectType>& removeList)
	//{
	//	vector<shared_ptr<Objects>> intersectionObjects = queryIntersections(eatObj->getBounds());
	//	
	//	for (auto& obj : intersectionObjects)
	//	{
	//		if (obj->checkEaten(eatObj.get()))
	//		{
	//			remove(obj->getID());
	//			removeList[obj->getID()] = obj->getType();
	//		}
	//	}
	//}*/

	//bool RTree::findObjectAndNode(shared_ptr<RTreeNode> node, const std::string& id, shared_ptr<RTreeNode>& outLeaf, std::shared_ptr<Objects>& outObj)
	//{
	//	if (!node) return false;

	//	if (node->leaf)
	//	{
	//		for (auto& e : node->entries)
	//		{
	//			if (auto obj = e.lock())
	//			{
	//				if (obj->getID() == id)
	//				{
	//					outLeaf = node;
	//					outObj = obj;
	//					return true;
	//				}
	//			}
	//		}
	//		return false;
	//	}
	//	else
	//	{
	//		for (auto& child : node->children)
	//		{
	//			if (findObjectAndNode(child, id, outLeaf, outObj))
	//			{
	//				return true;
	//			}
	//		}
	//		return false;
	//	}
	//}


	//bool RTree::removeEntry(shared_ptr<RTreeNode> node, const std::string& id)
	//{
	//	if (!node) return false;

	//	if (node->leaf)
	//	{
	//		auto it = find_if(node->entries.begin(), node->entries.end(),
	//			[&](const weak_ptr<Objects>& e) { return e.lock()->getID() == id; });
	//		if (it != node->entries.end())
	//		{
	//			iter_swap(it, node->entries.end() - 1);
	//			node->entries.pop_back();

	//			if (node->entries.empty())
	//			{
	//				removeEmptyNodes(node);
	//			}
	//			else
	//			{
	//				node->recalcMBR();
	//				adjustTree(node);
	//			}
	//			return true;
	//		}
	//		return false;
	//	}
	//	else
	//	{
	//		for (auto it = node->children.begin(); it != node->children.end(); ++it)
	//		{
	//			if (removeEntry(*it, id))
	//			{
	//				return true;
	//			}
	//		}
	//		return false;
	//	}
	//}

	//void RTree::removeEmptyNodes(shared_ptr<RTreeNode> node)
	//{
	//	if (!node) return;

	//	if ((node->leaf && node->entries.empty()) || (!node->leaf && node->children.empty()))
	//	{
	//		shared_ptr<RTreeNode> parent = node->parent.lock();
	//		if (parent)
	//		{
	//			auto it = std::find(parent->children.begin(), parent->children.end(), node);
	//			if (it != parent->children.end())
	//			{
	//				parent->children.erase(it);
	//			}

	//			parent->recalcMBR();
	//			adjustTree(parent);
	//			removeEmptyNodes(parent);
	//		}
	//		else
	//		{
	//			root = nullptr;
	//		}
	//	}
	//}

namespace
{
	//вспомогательные функции
	float area(const FloatRect& r)
	{
		return r.width * r.height;
	}

	sf::FloatRect unionRect(const sf::FloatRect& a, const sf::FloatRect& b)
	{
		float left = min(a.left, b.left);
		float top = min(a.top, b.top);
		float right = max(a.left + a.width, b.left + b.width);
		float bottom = max(a.top + a.height, b.top + b.height);
		return FloatRect(left, top, right - left, bottom - top);
	}

}
	


	Node::Node(bool leaf)
		:isLeaf(leaf), mbrDirty(true)
	{
		
	}
	sf::FloatRect Node::getMBR() const
	{
		if (mbrDirty || entries.empty())
		{
			cachedMBR = calculateMBR();
			mbrDirty = false;
		}
		return cachedMBR;
	}

	sf::FloatRect Node::calculateMBR() const
	{
		if (entries.empty()) return FloatRect();
		sf::FloatRect mbr = entries[0].mbr;
		for (size_t i = 1; i < entries.size(); ++i) {
			mbr = unionRect(mbr, entries[i].mbr);
		}
		return mbr;
	}

	void Node::invalidateMBR()
	{
		mbrDirty = true;
	}

	void RTree::update(std::shared_ptr<Objects> obj)
	{
		FloatRect lmb = obj->getBounds();
		remove(obj->getID(), lmb);
		insert(lmb, obj);
	}

	RTree::RTree() : root(make_shared<Node>(true))
	{

	}
	
	std::shared_ptr<Node> RTree::chooseLeaf(std::shared_ptr<Node> node, const sf::FloatRect& mbr)
	{
		if (node->isLeaf) return node;

		float minIncrease = numeric_limits<float>::max();
		shared_ptr<Node> bestChild;
		for (auto& entry : node->entries)
		{
			FloatRect unionMBR = unionRect(entry.mbr, mbr);
			float increase = area(unionMBR) - area(entry.mbr);
			if (increase < minIncrease)
			{
				minIncrease = increase;
				bestChild = entry.children;
			}
		}
		return chooseLeaf(bestChild, mbr);
	}

	void RTree::splitNode(std::shared_ptr<Node> node)
	{

		size_t n = node->entries.size();
		size_t bestGroup1 = 0, bestGroup2 = 1;
		float maxWaste = -1;
		for (int i = 0; i < n; ++i)
		{
			for (int j = i + 1; j < n; ++j)
			{
				FloatRect group1MBR = node->entries[i].mbr;
				FloatRect group2MBR = node->entries[j].mbr;

				float waste = area(unionRect(group1MBR, group2MBR)) - area(group1MBR) - area(group2MBR);
				if (waste > maxWaste)
				{
					maxWaste = waste;
					bestGroup1 = i;
					bestGroup2 = j;
				}
			}
		}
		vector<Entry> group1, group2;
		group1.push_back(node->entries[bestGroup1]);
		group2.push_back(node->entries[bestGroup2]);
		sf::FloatRect mbr1 = group1.back().mbr;
		sf::FloatRect mbr2 = group2.back().mbr;

		for (int i = 0; i < n; ++i)
		{
			if (i == bestGroup1 || i == bestGroup2) continue;

			sf::FloatRect enlarged1 = unionRect(mbr1, node->entries[i].mbr);
			sf::FloatRect enlarged2 = unionRect(mbr2, node->entries[i].mbr);

			float d1 = area(enlarged1) - area(mbr1);
			float d2 = area(enlarged2) - area(mbr2);
			if (d1 < d2)
			{
				group1.push_back(node->entries[i]);
				mbr1 = enlarged1;
			}
			else
			{
				group2.push_back(node->entries[i]);
				mbr2 = enlarged2;
			}
		}

		auto newNode = make_shared<Node>(node->isLeaf);

		newNode->entries = std::move(group2);
		newNode->parent = node->parent;

		if (!node->isLeaf)
		{
			for (auto& e : newNode->entries)
			{
				e.children->parent = newNode;
			}
		}

		node->entries = std::move(group1);

		if (auto p = node->parent.lock())
		{
			Entry newEntry = { weak_ptr<Objects>(), newNode->getMBR(), newNode };
			p->entries.push_back(newEntry);
			if (p->entries.size() > MAX_ENTRIES) splitNode(p);
		}
		else
		{
			auto newRoot = make_shared<Node>(false);
			newRoot->entries.push_back({ weak_ptr<Objects>(), newNode->getMBR(), newNode });
			newRoot->entries.push_back({ weak_ptr<Objects>(), newNode->getMBR(), node });
			node->parent = newRoot;
			newNode->parent = newRoot;
			root = newRoot;
		}

		if (auto p = node->parent.lock()) p->invalidateMBR();
	}

	void RTree::adjustTree(std::shared_ptr<Node> node)
	{
		if (!node) return;

		node->invalidateMBR();
		adjustTree(node->parent.lock());
		//for (auto& e : node->entries)
		//{
		//	if (e.children == node)
		//	{
		//		e.mbr = node->getMBR();
		//		break;
		//	}
		//}
		//adjustTree(node->parent.lock());
	}
	
	void RTree::insert(const sf::FloatRect& mbr, std::shared_ptr<Objects> obj)
	{
		if (!obj) return;


		Entry entry = {obj, mbr, nullptr};
		if (root->entries.empty())
		{
			root->entries.push_back(entry);
			root->invalidateMBR();
			return;
		}

		shared_ptr<Node> leafN = chooseLeaf(root, mbr);
		leafN->entries.push_back(entry);
		leafN->invalidateMBR();
		if (leafN->entries.size() > MAX_ENTRIES)
		{
			splitNode(leafN);
		}
		adjustTree(leafN->parent.lock());
	}

	bool RTree::remove(const std::string& id, const sf::FloatRect& localMBR)
	{
		return removeRecursive(root, id, localMBR);
	}

	bool RTree::removeRecursive(std::shared_ptr<Node> node, const std::string& id, const sf::FloatRect& localMBR)
	{
		if (node->isLeaf)
		{
			for (auto it = node->entries.begin(); it != node->entries.end(); ++it)
			{
				if (auto obj = it->obj.lock(); obj && obj->getID() == id)
				{
					node->entries.erase(it);
					node->invalidateMBR();
					collapseNode(node);
					return true;
				}
			}
			return false;
		}

		for (auto it = node->entries.begin(); it != node->entries.end(); ++it)
		{
			auto& entry = *it;

			if (true)/*entry.mbr.intersects(localMBR)*/ //цикличный интерсект
			{
				if (removeRecursive(entry.children, id, localMBR))
				{
					if (node->isLeaf)
					{
						collapseNode(node);
						return true;
					}
				}
			}
		}
		return false;
	}

	void RTree::collapseNode(std::shared_ptr<Node> node)
	{
		auto p = node->parent.lock();
		if (!p) return;
		
		int sizeEntries = 0;
		for (auto& e : p->entries)
		{
			if (!e.children->isLeaf)
			{
				return;
			}	
			sizeEntries += e.children->entries.size();
		}

		if (sizeEntries > MIN_ENTRIES) return;

		vector<Entry> tempEntries;
		tempEntries.reserve(sizeEntries);
		for (auto& e : p->entries)
		{
			tempEntries.insert(tempEntries.end(), make_move_iterator(e.children->entries.begin()), 
				make_move_iterator(e.children->entries.end()));
		}
		p->entries = std::move(tempEntries);
		p->isLeaf = true;

		adjustTree(p);
	}

	std::vector<std::shared_ptr<Objects>> RTree::queryIntersections(const sf::FloatRect& queryRect) const
	{
		vector<shared_ptr<Objects>> results;
		queryIntersectionsRecursive(root, queryRect, results);
		return results;
	}

	void RTree::queryIntersectionsRecursive(std::shared_ptr<Node> node, const sf::FloatRect& queryRect, std::vector<std::shared_ptr<Objects>>& results) const
	{
		for (auto& entry : node->entries)
		{
			//TODO: цикличные пересечения квадратов
			
			
			if (node->isLeaf)
			{
				auto obj = entry.obj.lock();
				if(obj) results.push_back(obj);
			}
			else
			{
				queryIntersectionsRecursive(entry.children, queryRect, results);
			}
		}
	}

}
