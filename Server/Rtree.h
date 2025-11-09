#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "MoveObjectServer.h"

namespace Server
{
	class Node;

	struct Entry
	{
		std::weak_ptr<Objects> obj;
		sf::FloatRect mbr;
		std::shared_ptr<Node> children;
	};

	class Node
	{
	public:
		bool isLeaf;
		std::vector<Entry> entries;
		std::weak_ptr<Node> parent;
		mutable sf::FloatRect cachedMBR;
		mutable bool mbrDirty;

		Node(bool leaf = true);
		
		sf::FloatRect getMBR() const;

		void invalidateMBR();
	private:
		sf::FloatRect calculateMBR() const;
	};

	class RTree
	{
	public:
		RTree();
		void insert(const sf::FloatRect& mbr, std::shared_ptr<Objects> obj);
		bool remove(const std::string& id, const sf::FloatRect& localMBR);
		void update(std::shared_ptr<Objects> obj);
		/*void tryEat(std::shared_ptr<MoveObject> eatObj, std::unordered_map<std::string, ObjectType>& removeList);*/
		std::vector<std::shared_ptr<Objects>> queryIntersections(const sf::FloatRect& queryRect) const;
	private:
		shared_ptr<Node> root;

		std::shared_ptr<Node> chooseLeaf(std::shared_ptr<Node> node, const sf::FloatRect& mbr);
		bool removeRecursive(std::shared_ptr<Node> node, const std::string& id, const sf::FloatRect& localMBR);
		void collapseNode(std::shared_ptr<Node> node);
		/*void insertRecursive(std::shared_ptr<Node> node, const Entry& entry);*/
		void splitNode(std::shared_ptr<Node> node);
		void adjustTree(std::shared_ptr<Node> node);
		void queryIntersectionsRecursive(std::shared_ptr<Node> node, const sf::FloatRect& queryRect, std::vector<std::shared_ptr<Objects>>& results) const;

	};

	////class RTree
	////{
	////public:
	////	RTree();
	////	void insert(std::shared_ptr<Objects> obj);
	////	bool remove(const std::string& id);
	////	void update(std::shared_ptr<Objects> obj);
	////	/*void tryEat(std::shared_ptr<MoveObject> eatObj, std::unordered_map<std::string, ObjectType>& removeList);*/
	////	std::vector<std::shared_ptr<Objects>> queryIntersections(const sf::FloatRect& queryRect) const;
	////private:
	////	class RTreeNode
	////	{
	////	public:
	////		RTreeNode(bool isLeaf, std::shared_ptr<RTreeNode> _parent = nullptr);
	////		void recalcMBR();
	////	public:
	////		bool leaf;
	////		std::vector<std::weak_ptr<Objects>> entries;
	////		std::vector<std::shared_ptr<RTreeNode>> children;
	////		sf::FloatRect mbr;

	////		std::weak_ptr<RTreeNode> parent;

	////		static const size_t MAX_ENTRIES = 4;
	////	};

	////	static float area(const sf::FloatRect& r);
	////	static sf::FloatRect combine(const sf::FloatRect& a, const sf::FloatRect& b);
	////	static float enlargement(const sf::FloatRect& mbr, const sf::FloatRect& r);

	////	std::shared_ptr<RTreeNode> chooseLeaf(std::shared_ptr<RTreeNode>, std::shared_ptr<Objects> entry);
	////	void insertEntry(std::shared_ptr<RTreeNode> node, std::shared_ptr<Objects> entry);
	////	void splitNode(std::shared_ptr<RTreeNode> node);
	////	void adjustTree(std::shared_ptr<RTreeNode> node);
	////	bool removeEntry(std::shared_ptr<RTreeNode> node, const std::string& id);
	////	void removeEmptyNodes(std::shared_ptr<RTreeNode> node);
	////	void queryIntersectionsRecursive(std::shared_ptr<RTreeNode> node, const sf::FloatRect& queryRect, std::vector<std::shared_ptr<Objects>>& results) const;

	////	bool findObjectAndNode(std::shared_ptr<RTreeNode> node, const std::string& id, std::shared_ptr<RTreeNode>& outLeaf, std::shared_ptr<Objects>& outObj);
	////private:

	////	std::shared_ptr<RTreeNode> root;

	////};

}

