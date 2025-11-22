#include "BaseOld.h"

#include "Generator.h"
#include "Constants.h"

#include <limits>
#include <iostream>
#include <cmath>
#include <cassert>
constexpr double PI = 3.14159265358979323846;


using namespace std;

namespace Server
{
	float GetLen(const Vector2f& p1, const Vector2f& p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}

	float GetLen(const Vector2f& vector) {
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	float GetCyclicDist(const Vector2f& p1, const Vector2f& p2, float sizeW, float sizeH)
	{
		Vector2f diff = GetCyclicDiff(p1, p2, sizeW, sizeH);
		return hypot(diff.x, diff.y);
	}

	bool containsRect(const sf::FloatRect& outer, const sf::FloatRect& inner)
	{
		return inner.left >= outer.left &&
			(inner.left + inner.width) <= (outer.left + outer.width) &&
			inner.top >= outer.top &&
			(inner.top + inner.height) <= (outer.top + outer.height);
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

	Vector2f calcAttractionForce(const sf::Vector2f& dir, float koef)
	{
		return dir * koef;
	}


	Vector2f calcAcceleration(const Vector2f& F, const float mass)
	{
		return F / mass;
	}

	Vector2f calcSpeed(const Vector2f& v0, const Vector2f& a, int diff, float maxV, float koef)
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


	float toroidalDiff(float diff, float size)
	{
		float half = size / 2.f;
		if (diff > half) return diff - size;
		else if (diff < -half) return diff + size;
		return diff;
	}

	sf::Vector2f GetCyclicDiff(const Vector2f& from, const Vector2f& to, float sizeW, float sizeH)
	{
		float diffX = to.x - from.x;
		float dx = toroidalDiff(diffX, sizeW);
		float diffY = to.y - from.y;
		float dy = toroidalDiff(diffY, sizeH);
		return { dx, dy };
	}

	Vector2f generateDir()
	{
		float angle = genNumber<float>(0.f, 2.f * ServerConstants::PI);
		return { cos(angle), sin(angle) };
	}


	bool pointInSegment(float p, float segmentStart, float segmentLength, float size)
	{
		assert(size > 0 && segmentLength >= 0 && "Invalid parameters");
		float wrappedP = wrapCoordinate(p, size);
		float wrappedStart = wrapCoordinate(segmentStart, size);
		float wrappedEnd = wrapCoordinate(segmentStart + segmentLength, size);

		if (wrappedEnd >= wrappedStart) {
			return wrappedP >= wrappedStart && wrappedP <= wrappedEnd;
		}
		else {
			return wrappedP >= wrappedStart || wrappedP <= wrappedEnd;
		}
	}

	bool cyclicIntersectsRect(const sf::FloatRect& r1, const sf::FloatRect& r2, float sizeW, float sizeH)
	{
		return (pointInSegment(r1.left, r2.left, r2.width, sizeW) || pointInSegment(r1.left + r1.width, r2.left, r2.width, sizeW))
			&& (pointInSegment(r1.top, r2.top, r2.height, sizeH) || pointInSegment(r1.top + r1.height, r2.top, r2.height, sizeH));
	}

	float wrapCoordinate(float coord, float size)
	{
		assert(size > 0 && "size must be positive");
		float result = fmod(coord, size);
		if (result < 0) result += size;
		return result;
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
		if (len < 1e-6) return { 0.f, 0.f };
		return { vector.x / len, vector.y / len };
	}

	bool circleIntersectCircle(const sf::Vector2f& c1, const float r1, const sf::Vector2f& c2, const float r2)
	{
		float dist = GetLen(c1, c2) + 0.001f;
		return dist <= (r1 + r2);
	}
}


