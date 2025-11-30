#include "MoveObject.h"

constexpr float MAX_DIFF = 300.f;

MoveObject::MoveObject(Vector2f center, float radius, string id): Objects(center, radius,id), V(0.f, 0.f), Mouse(0.f, 0.f), maxV(0.f)
{

}

void MoveObject::TimeElapsed(int diff)
{
	/*float len = GetLen(difference);
	float giveLen = 0.035f * diff;
	if (len > 0)
	{
		if (giveLen > len)
		{
			Center += difference;
			difference = { 0.f, 0.f };
		}
		else
		{
			Vector2f give = difference * giveLen / len;
			Center += give;
			difference -= give;
		}
	}*/
	Center += difference;
	difference = { 0.f, 0.f };
	Center += V * (float)diff;
}

void MoveObject::setSpeed(Vector2f newSpeed)
{
	V = newSpeed;
}

Vector2f MoveObject::calcAttractionForce(const sf::Vector2f& dir, float koef)
{
	return dir * koef;
}


Vector2f MoveObject::calcAcceleration(const Vector2f& F, const float mass)
{
	return F / mass;
}

Vector2f MoveObject::calcSpeed(const Vector2f& v0, const Vector2f& a, int diff, float maxV, float koef)
{
	Vector2f v{ 0.f,0.f };
	v = v0 + a * (float)diff;
	float lenV = hypot(v.x, v.y);
	if (lenV > maxV)
	{
		v.x *= maxV / lenV;
		v.y *= maxV / lenV;
	}
	return v;
}

float MoveObject::toroidalDiff(float diff, float size)
{
	float half = size / 2.f;
	if (diff > half) return diff - size;
	else if (diff < -half) return diff + size;
	return diff;
}

sf::Vector2f MoveObject::GetCyclicDiff(const Vector2f& from, const Vector2f& to, float sizeW, float sizeH)
{
	float diffX = to.x - from.x;
	float dx = toroidalDiff(diffX, sizeW);
	float diffY = to.y - from.y;
	float dy = toroidalDiff(diffY, sizeH);
	return { dx, dy };
}

void MoveObject::setDifference(Vector2f diff)
{
	if (GetLen(diff) > MAX_DIFF)
	{
		Center += diff;
		difference = { 0, 0 };
	}
	else
	{
		difference = diff;
	}
}

Vector2f MoveObject::getDiff()
{
	return difference;
}

float MoveObject::GetLen(const Vector2f& p1, const Vector2f& p2, const Vector2f& mapSize)
{
	float halfX = mapSize.x / 2.f;
	float halfY = mapSize.y / 2.f;
	float lenX;
	float lenY;
	if (fabs(p2.x - p1.x) > halfX)
	{
		lenX = pow(fabs(p2.x - p1.x) - halfX, 2);
	}
	else
	{
		lenX = pow(p2.x - p1.x, 2);
	}

	if (fabs(p2.y - p1.y) > halfY)
	{
		lenY = pow(fabs(p2.y - p1.y) - halfY, 2);
	}
	else
	{
		lenY = pow(p2.y - p1.y, 2);
	}
	return sqrt(lenX + lenY);
}

float MoveObject::GetLen(const Vector2f& vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2f MoveObject::identityVector(const sf::Vector2f& vec)
{
	float len = GetLen(vec);
	if (len < 1e-6) return { 0.f, 0.f };
	return { vec.x / len, vec.y / len };
}

Vector2f MoveObject::getIdentityVector(const Vector2f& vector)
{
	float len = GetLen(vector);
	Vector2f Vec1;
	if (len != 0)
	{
		Vec1 = vector / len;
	}
	else
	{
		float angle = (float)(rand() % 360) / 180.0f * 3.1415926;
		Vec1 = Vector2f(cos(angle), sin(angle));
	}
	return Vec1;
}

void MoveObject::setDirection(const sf::Vector2f& dir)
{
	Mouse = dir;
}

void MoveObject::setMaxV(float _maxV)
{
	maxV = _maxV;
}

Vector2f MoveObject::getDirection() const
{
	return Mouse;
}
