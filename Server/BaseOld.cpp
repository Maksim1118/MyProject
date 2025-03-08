#include "BaseOld.h"

float GetLen(const Vector2f& p1,const Vector2f &p2) 
{
	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

float GetLen(const Vector2f& vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2f getIdentityVector(const Vector2f& vector)
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
