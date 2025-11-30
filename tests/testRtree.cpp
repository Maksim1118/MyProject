#include "ObjectsServer.h"
#include "IRegistrator.h"
#include "RTree.h"
#include <gtest/gtest.h>

using namespace Server;
using namespace sf;
using namespace std;

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


class TestObj : public Objects
{
public:
	TestObj(const int id, const FloatRect& bounds) : Objects(reinterpret_cast<IRegistrator*>(1), Vector2f(0.f, 0.f), 0.f), m_id(id), m_bounds(bounds)
	{

	}

	bool Eat(Objects& obj)override { return false; };
	bool checkEaten(Objects& eatingObj) override { return false; };
	sf::FloatRect getMBR()const override { return m_bounds; };
	void TimeElapsed(int diff)override {};
	nlohmann::json toStaticJson() const override { return nlohmann::json(); };
	nlohmann::json toPersistentJson() const override { return nlohmann::json(); };

private:
	int m_id;
	FloatRect m_bounds;
};


class RTreeHier : public RTree 
{
public:
	using RTree::root;
	using RTree::MAX_ENTRIES;
	using RTree::MIN_ENTRIES;
};

class RTreeTest : public ::testing::Test 
{
protected:
	void SetUp() override
	{
		tree = make_unique<RTreeHier>();
	}

	std::unique_ptr<RTreeHier> tree;

};

//пустое дерево
TEST_F(RTreeTest, EmptyTree)
{
	EXPECT_TRUE(tree->root->isLeaf);
	EXPECT_TRUE(tree->root->entries.empty());
	EXPECT_TRUE(tree->root->parent.expired());

	FloatRect mbr = tree->root->getMBR();
	EXPECT_EQ(mbr.left, 0);
	EXPECT_EQ(mbr.top, 0);
	EXPECT_EQ(mbr.width, 0);
	EXPECT_EQ(mbr.height, 0);
}

//дерево с одним объектом
TEST_F(RTreeTest, SingleObj)
{
	auto obj = make_shared<TestObj>(1, FloatRect(20, 20, 100, 100));
	tree->insert(obj->getMBR(), obj);

	EXPECT_TRUE(tree->root->isLeaf);
	EXPECT_EQ(tree->root->entries.size(), 1);
	EXPECT_TRUE(tree->root->parent.expired());

	FloatRect mbr = tree->root->getMBR();
	EXPECT_EQ(mbr, FloatRect(20, 20, 100, 100));

	auto results = tree->queryIntersections(FloatRect(0, 0, 300, 300));
	EXPECT_EQ(results.size(), 1);
}

//проверка функции объединения, объекты не выходят за границы
TEST_F(RTreeTest, UnionTest1)
{
	FloatRect a(50, 50, 100, 100);
	FloatRect b(20, 20, 300, 150);
	FloatRect unionMbr = unionRect(a, b);

	EXPECT_EQ(unionMbr, FloatRect(20, 20, 300, 150));
}

TEST_F(RTreeTest, UnionTest2)
{
	FloatRect a(20, 20, 20, 20);
	FloatRect b(300, 150, 20, 20);
	FloatRect unionMbr = unionRect(a, b);

	EXPECT_EQ(unionMbr, FloatRect(20, 20, 300, 150));
}

//дерево со множеством объектов
TEST_F(RTreeTest, ObjectsSplit)
{
	FloatRect mbr;
	auto obj1 = make_shared<TestObj>(1, FloatRect(20 + 0 * 120, 20, 100, 100));
	tree->insert(obj1->getMBR(), obj1);

	auto obj2 = make_shared<TestObj>(2, FloatRect(20 + 1 * 120, 20, 100, 100));
	tree->insert(obj2->getMBR(), obj2);

	auto obj3 = make_shared<TestObj>(3, FloatRect(20 + 0 * 80, 200, 50, 50));
	tree->insert(obj3->getMBR(), obj3);

	auto obj4 = make_shared<TestObj>(4, FloatRect(20 + 1 * 80, 200, 50, 50));
	tree->insert(obj4->getMBR(), obj4);

	auto obj5 = make_shared<TestObj>(5, FloatRect(50 + 0 * 120, 300, 70, 70));
	tree->insert(obj5->getMBR(), obj5);

	auto obj6 = make_shared<TestObj>(6, FloatRect(50 + 1 * 120, 300, 70, 70));
	tree->insert(obj6->getMBR(), obj6);

	EXPECT_FALSE (tree->root->isLeaf);
	EXPECT_LE(tree->root->entries.size(), RTreeHier::MAX_ENTRIES);

	FloatRect mbr2 = tree->root->getMBR();
	EXPECT_EQ(mbr2, FloatRect(20, 20, 220, 350));

	auto results = tree->queryIntersections(FloatRect(0, 0,300, 330));
	EXPECT_EQ(results.size(), 6);
}