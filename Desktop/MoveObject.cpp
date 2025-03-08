#include "MoveObject.h"

int MoveObject::lastId = 0;

MoveObject::MoveObject(Vector2f center, float radius, int id): Objects(center, radius,id), V(0.f, 0.f), Mouse(0.f, 0.f)
{

}

void MoveObject::TimeElapsed(int diff)
{
	float len = GetLen(difference);
	float giveLen = 0.055f * diff;
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
	}
	Center += V * (float)diff;
}

void MoveObject::setSpeed(Vector2f newSpeed)
{
	V = newSpeed;
}

void MoveObject::setDifference(Vector2f diff)
{
	difference = diff;
}

void MoveObject::setCenter(Vector2f newCenter)
{
	Center = newCenter;
}

Vector2f MoveObject::getDiff()
{
	return difference;
}

float MoveObject::GetLen(const Vector2f& p1, const Vector2f& p2)
{
	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

float MoveObject::GetLen(const Vector2f& vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
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