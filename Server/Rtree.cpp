#include "Rtree.h"

#include "ObjectsServer.h"

#include "BaseOld.h"
#include "Constants.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace sf;

namespace Server
{

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

	vector<FloatRect> unionRectCyclic(const sf::FloatRect& a, const sf::FloatRect& b, float sizeW, float sizeH)
	{
		vector<FloatRect> aParts = normalizeRect(a, sizeW, sizeH);
		vector<FloatRect> bParts = normalizeRect(b, sizeW, sizeH);

		vector<FloatRect> result;
		for (const auto& pA : aParts)
		{
			for (const auto& pB : bParts)
			{
				FloatRect uni = unionRect(pA, pB);
				result.push_back(uni);
			}
		}

		float totalArea = 0;
		for (const auto& r : result) totalArea += r.width * r.height;
		if (totalArea >= sizeW * sizeH * 0.8f) {
			result = { sf::FloatRect(0, 0,sizeW, sizeH) };
		}
		return result;
	}

}

	sf::FloatRect Entry::getMBR() const
	{
		if (!children)
		{
			return obj.lock()->getMBR();
		}
		else
		{
			return children->getMBR();
		}
	}

	Node::Node(bool leaf)
		:isLeaf(leaf), mbrDirty(true)
	{
		
	}


	sf::FloatRect Node::getMBR() const
	{
		if (mbrDirty)
		{
			cachedMBR = calculateMBR();
			mbrDirty = false;
		}
		return cachedMBR;
	}

	sf::FloatRect Node::calculateMBR() const
	{
		if (entries.empty()) return FloatRect();
		sf::FloatRect mbr;
		mbr = entries[0].getMBR();

		for (size_t i = 1; i < entries.size(); ++i)
		{
			mbr = unionRect(mbr, entries[i].getMBR());
		}
		
		return mbr;
	}

	void Node::invalidateMBR()
	{
		mbrDirty = true;
	}

	void printFloatRect(FloatRect& r)
	{
		cout << '(' << r.left << ',' << r.left + r.width << ',' << r.top << ',' << r.top + r.height << ')';
	}

	void Node::print(int level)
	{
		cout << string(level * 2, ' ');
		FloatRect mbr = getMBR();
		cout << isLeaf << ' ';
		printFloatRect(mbr);
		cout << endl;
		for (auto e : entries)
		{
			cout << string(level * 2 + 2, ' ');
			printFloatRect(e.getMBR());
			cout << endl;
			if (!isLeaf)
			{
				e.children->print(level + 1);
			}
		}	
	}

	void RTree::update(std::shared_ptr<Objects> obj)
	{
		FloatRect lmb = obj->getMBR();
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
			FloatRect unionMBR = unionRect(entry.getMBR(), mbr);
			float increase = area(unionMBR) - area(entry.getMBR());
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
				FloatRect group1MBR = node->entries[i].getMBR();
				FloatRect group2MBR = node->entries[j].getMBR();

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
		sf::FloatRect mbr1 = group1.back().getMBR();
		sf::FloatRect mbr2 = group2.back().getMBR();

		for (int i = 0; i < n; ++i)
		{
			if (i == bestGroup1 || i == bestGroup2) continue;

			sf::FloatRect enlarged1 = unionRect(mbr1, node->entries[i].getMBR());
			sf::FloatRect enlarged2 = unionRect(mbr2, node->entries[i].getMBR());

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
		newNode->invalidateMBR();

		newNode->parent = node->parent;

		if (!node->isLeaf)
		{
			for (auto& e : newNode->entries)
			{
				e.children->parent = newNode;
			}
		}

		node->entries = std::move(group1);
		node->invalidateMBR();

		if (auto p = node->parent.lock())
		{
			Entry newEntry = { weak_ptr<Objects>(), newNode };
			p->entries.push_back(newEntry);
			if (p->entries.size() > MAX_ENTRIES) splitNode(p);
		}
		else
		{
			auto newRoot = make_shared<Node>(false);
			newRoot->entries.push_back({ weak_ptr<Objects>(),newNode });
			newRoot->entries.push_back({ weak_ptr<Objects>(),node });
			node->parent = newRoot;
			newNode->parent = newRoot;
			root = newRoot;
			newRoot->invalidateMBR();
		}

		if (auto p = node->parent.lock()) p->invalidateMBR();
	}

	void RTree::adjustTree(std::shared_ptr<Node> node)
	{
		if (!node) return;

		node->invalidateMBR();
		adjustTree(node->parent.lock());
	}
	
	void RTree::insert(const sf::FloatRect& mbr, std::shared_ptr<Objects> obj)
	{
		if (!obj) return;


		Entry entry = {obj, nullptr};
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
		adjustTree(leafN);
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


	void RTree::print()
	{
		root->print();
	}

	void RTree::queryIntersectionsRecursive(std::shared_ptr<Node> node, const sf::FloatRect& queryRect, std::vector<std::shared_ptr<Objects>>& results) const
	{
		if (!queryRect.intersects(node->getMBR()))
		{
			return;
		}
		
		for (auto& entry : node->entries)
		{
			//TODO: цикличные пересечения квадратов
			
			
			if (node->isLeaf)
			{
				auto obj = entry.obj.lock();
				if (obj)
				{
					/*if (obj->getBounds().intersects(queryRect))
					{
						results.push_back(obj);
					}*/
					results.push_back(obj);
				}
			}
			else
			{
				queryIntersectionsRecursive(entry.children, queryRect, results);
			}
		}
	}
}
