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
		std::vector<std::shared_ptr<Objects>> queryIntersections(const sf::FloatRect& queryRect) const;
	private:
		shared_ptr<Node> root;

		std::shared_ptr<Node> chooseLeaf(std::shared_ptr<Node> node, const sf::FloatRect& mbr);
		bool removeRecursive(std::shared_ptr<Node> node, const std::string& id, const sf::FloatRect& localMBR);
		void collapseNode(std::shared_ptr<Node> node);
		void splitNode(std::shared_ptr<Node> node);
		void adjustTree(std::shared_ptr<Node> node);
		void queryIntersectionsRecursive(std::shared_ptr<Node> node, const sf::FloatRect& queryRect, std::vector<std::shared_ptr<Objects>>& results) const;

	};
}

