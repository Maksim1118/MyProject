#include "FeedServer.h"

namespace Server
{
	Feed::Feed() : MoveObject(Vector2f(0, 0), _FoodMass), parentCenter( 0.f,0.f ), parentRadius(0.f), m_IsUnderHero(true)
	{
		colorNum = 2;
		state = States::EATEN;
	}

	void Feed::TimeElapsed(int diff)
	{
		if (GetLen(parentCenter, getCenter()) > parentRadius * 1.2)
		{
			if (m_IsUnderHero)
			{
				state = States::LIVE;
				m_IsUnderHero = false;
			}
		}
		if (GetLen(parentCenter, getCenter()) > parentRadius)
		{
			float lenV = GetLen(V);
			float newlenV = lenV - 0.0005f * (float)diff;
			if (newlenV <= 0)
			{
				V = { 0,0 };
			}
			else
			{
				V *= newlenV / lenV;

			}
		}
		Move(V * (float)diff);
		cout << "Feed" << "    " ;
		cout << "FeedSpeed: " << V.x << "    " << V.y << "  ";
		cout << "FeedCoords: " << _center.x << "    " << _center.y << endl;
	}

	void Feed::setV(Vector2f& newV)
	{
		V = newV;
	}

	void Feed::setParentCenter(Vector2f& newCenter)
	{
		parentCenter = newCenter;
	}

	void Feed::setParentRadius(float newRadius)
	{
		parentRadius = newRadius;
	}

	bool Feed::checkEaten(MoveObject* obj)
	{
		if (state == States::EATEN || obj->state == States::EATEN)
			return false;
		if (obj->Eating((*this), -getRadius()))
		{
			return true;
		}
		return false;
	}
}
