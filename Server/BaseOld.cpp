#include "BaseOld.h"

#include <limits>
#include <cmath>
constexpr double PI = 3.14159265358979323846;


using namespace std;


float GetLen(const Vector2f& p1,const Vector2f &p2) 
{
	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

float GetLen(const Vector2f& vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float GetDist(const Vector2f& p1, const Vector2f& p2)
{
	/*float halfX = fieldWidth / 2.f;
	float halfY = fieldHeight / 2.f;
	float lenX;
	float lenY;

	float diffX = p2.x - p1.x;
	float absDiffX(fabs(diffX));
	if (absDiffX > halfX)
	{
		lenX = pow(fieldWidth - absDiffX, 2);
	}
	else
	{
		lenX = pow(diffX, 2);
	}

	float diffY = p2.y - p1.y;
	float absDiffY(fabs(diffY));

	if (absDiffY > halfY)
	{
		lenY = pow(fieldHeight - absDiffY, 2);
	}
	else
	{
		lenY = pow(diffY, 2);
	}
	return sqrt(lenX + lenY);*/
	Vector2f diff = GetDiff(p1, p2);
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

Vector2f findCyclicCenter(const vector<Vector2f>& centers)
{
	double sumCosX = 0, sumSinX = 0;
	double sumCosY = 0, sumSinY = 0;

	for (const auto& c : centers)
	{
		double angleX = (c.x / fieldWidth) * 2.0 * PI;
		sumCosX += cos(angleX);
		sumSinX += sin(angleX);

		double angleY = (c.y / fieldHeight) * 2.0 * PI;
		sumCosY += cos(angleY);
		sumSinY += sin(angleY);
	}

	double avgAngleX = atan2(sumSinX, sumCosX);
	if (avgAngleX < 0) avgAngleX += 2 * PI;
	
	double avgAngleY = atan2(sumSinY, sumCosY);
	if (avgAngleY < 0) avgAngleY += 2 * PI;

	float centerX = static_cast<float>((avgAngleX / (2.0 * PI) * fieldWidth));
	float centerY = static_cast<float>((avgAngleY / (2.0 * PI) * fieldHeight));
	return { centerX, centerY };
}

float distanceToMapBorder(const Vector2f& center, const Vector2f& dir)
{
	Vector2f d = identityVector(dir);
	if (d.x == 0.f && d.y == 0.f)
	{
		return -1;
	}

	float minT = numeric_limits<float>::max();

	if (d.x != 0.f)
	{
		float t = (0.f - center.x) / d.x;
		if (t >= 0.f)
		{
			float y = center.y + t * d.y;
			if (y >= 0.f && y <= fieldHeight && t < minT)
			{
				minT = t;
			}
		}
	}

	if (d.x != 0.f)
	{
		float t = (fieldWidth - center.x) / d.x;
		if (t >= 0.f)
		{
			float y = center.y + t * d.y;
			if (y >= 0.f && y <= fieldHeight && t < minT)
			{
				minT = t;
			}
		}
	}

	if (d.y != 0.f)
	{
		float t = (0 - center.y) / d.y;
		if (t >= 0.f)
		{
			float x = center.x + t * d.x;
			if (x >= 0.f && x <= fieldWidth && t < minT)
			{
				minT = t;
			}
		}
	}

	if (d.y != 0.f)
	{
		float t = (fieldHeight - center.y) / d.y;
		if (t >= 0.f)
		{
			float x = center.x + t * d.x;
			if (x >= 0.f && x <= fieldWidth && t < minT)
			{
				minT = t;
			}
		}
	}

	if (minT == numeric_limits<float>::max())
	{
		return -1.f;
	}
	return minT;
}

sf::Vector2f identityVector(sf::Vector2f vec)
{
	float len = GetLen(vec);
	if (len < 1e-6) return { 0.f, 0.f };
	return { vec.x / len, vec.y / len };
}

sf::Vector2f GetDiff(const Vector2f& p1, const Vector2f& p2)
{
	/*float halfX = fieldWidth / 2.f;
	float halfY = fieldHeight / 2.f;
	Vector2f res;
	
	if (p2.x - p1.x > halfX)
	{
		res.x = p1.x - p2.x + fieldWidth;
	}
	else if(p1.x - p2.x > halfX)
	{
		res.x = p1.x - p2.x - fieldWidth;
	}
	else
	{
		res.x = p1.x - p2.x;
	}

	if (p2.y - p1.y > halfY)
	{
		res.y = p1.y - p2.y + fieldHeight;
	}
	else if (p1.y - p2.y > halfY)
	{
		res.y = p1.y - p2.y - fieldHeight;
	}
	else
	{
		res.y = p1.y - p2.y;
	}

	return res;*/
	float halfX = fieldWidth / 2.f;
	float halfY = fieldHeight / 2.f;

	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	if (dx > halfX)
	{
		dx -= fieldWidth;
	}
	else if (dx < -halfX)
	{
		dx += fieldWidth;
	}

	if (dy > halfY)
	{
		dy -= fieldHeight;
	}
	else if (dy < -halfY)
	{
		dy += fieldHeight;
	}
	return { dx, dy };
}

sf::Vector2f normalizeCoord(const sf::Vector2f& vec)
{
	Vector2f res = vec;
	if (vec.x < 0)
	{
		res.x += fieldWidth;
	}
	else if (vec.x > fieldWidth)
	{
		res.x -= fieldWidth;
	}

	if (vec.y < 0)
	{
		res.y += fieldHeight;
	}
	else if (vec.y > fieldHeight)
	{
		res.y -= fieldHeight;
	}
	return res;
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

bool circleIntersectCircle(const sf::Vector2f& c1, const float r1, const sf::Vector2f& c2, const float r2)
{
	float dist = GetLen(c1, c2) + 0.001f;
	return dist <= (r1 + r2);
}

