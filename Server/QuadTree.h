#pragma once

#include <vector>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

namespace Server
{
	struct ObjectData
	{
		std::string id;
		sf::FloatRect bounds;
		sf::Vector2f center;
		float radius;
	};

	class QuadTree
	{
	public:

		void insert(const ObjectData& obj);
		int getIndex(const sf::FloatRect bounds) const;
		void split();
		bool remove(const std::string id);
		std::vector<ObjectData> retrive(sf::FloatRect& queryRect) const;
		/*sf::FloatRect getBounds() const;
		std::vector<sf::FloatRect> getAllBounds() const;
		bool isIntersect(const sf::Vector2f centerObj, const float radiusObj) const;*/

		QuadTree(int pLevel, sf::FloatRect pBounds);
	private:
		static const int MAX_OBJECTS = 4;
		static const int MAX_LEVELS = 5;

		int level;
		std::vector<ObjectData> objects;
		std::unique_ptr<QuadTree> nodes[4];
		sf::FloatRect bounds;

	};
}