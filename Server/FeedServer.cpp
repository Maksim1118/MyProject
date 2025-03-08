#include "FeedServer.h"

namespace Server
{
	Feed::Feed() : Food(),Objects(Vector2f(0.f, 0.f), _FoodMass), MoveObject(Vector2f(0, 0), _FoodMass), parentCenter( 0.f,0.f ), parentRadius(0.f)
	{
		eatable = false;
		_mass = 25.f;
		colorNum = 2;
	}

	void Feed::TimeElapsed(int diff)
	{
		if (GetLen(parentCenter, getCenter()) > parentRadius * 1.2)
		{
			eatable = true;
		}
		if (GetLen(parentCenter, getCenter()) > parentRadius)
		{
			float lenV = GetLen(V);
			float newlenV = lenV - 0.001f * diff;
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
}
