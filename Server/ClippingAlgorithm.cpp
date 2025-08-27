#include "ClippingAlgorithm.h"

using namespace sf;
using namespace std;

vert* createPolygon(const vector<Vector2f>& points)
{
	if (points.empty())
	{
		return nullptr;
	}

	vert* head = new vert(points[0]);
	vert* prev = head;
	for (int i = 1; i < points.size(); ++i)
	{
		vert* v = new vert(points[i]);
		v->prev = prev;
		prev->next = v;
		prev = v;
	}
	head->prev = prev;
	prev->next = head;
	return head;
}

void deletePolygon(vert* poly)
{
	if (!poly)
	{
		return;
	}
	vert* start = poly;
	vert* cur = poly->next;
	while (cur != start)
	{
		vert* next = cur->next;
		delete cur;
		cur = next;
	}
	delete start;
}

bool segmentIntersect(const Vector2f& p1, const Vector2f& p2, const Vector2f& q1, const Vector2f& q2, float& t1, float &t2, Vector2f& inter)
{
	Vector2f d1 = p2 - p1;
	Vector2f d2 = q2 - q1;
	float d1Xd2 = d1.x * d2.y - d2.x * d1.y;
	if (fabs(d1Xd2) < 1e-8)
	{
		return false;
	}
	return true;
}

