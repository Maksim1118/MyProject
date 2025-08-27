#include "Map.h"

#include <stdexcept>

using namespace sf;
using namespace std;

namespace Server
{
	Vector2i Map::findSegmentIndices(Vector2f pos)
	{
		int indexX = static_cast<int>(pos.x / segmentSize.x);
		int indexY = static_cast<int>(pos.y / segmentSize.y);
		return { indexX, indexY };
	}

	vector<Vector2i> Map::objectInSegments(Vector2f center, float radius)
	{
		vector<Vector2i> res;

		Vector2f topLeft(center.x - radius, center.y - radius);
		Vector2f topRight(center.x + radius, center.y + radius);

		Vector2i start = findSegmentIndices(topLeft);
		Vector2i end = findSegmentIndices(topRight);


		start.x = clamp(start.x, 0, segmentCol - 1);
		start.y = clamp(start.y, 0, segmentRows - 1);
		end.x = clamp(end.x, 0, segmentCol - 1);
		end.y = clamp(end.y, 0, segmentRows - 1);

		for (int row = start.y; row <= end.y; ++row)
		{
			for (int col = start.x; col <= end.x; ++col)
			{
				float segX = col * segmentSize.x;
				float segY = row * segmentSize.y;

				float closestX = clamp(center.x, segX, segX + segmentSize.x);
				float closestY = clamp(center.y, segY, segY + segmentSize.y);

				float deltaX = center.x - closestX;
				float deltaY = center.y - closestY;

				if ((pow(deltaX, 2) + pow(deltaY, 2)) <= pow(radius, 2))
				{
					res.push_back({ col, row });
				}
			}
		}
		return res;
	}

	segmentInterval Map::getSegmentInterval(sf::Vector2f center, sf::Vector2f sizeParamObj) const
	{
		Vector2f leftUpPos = { center - sizeParamObj * 0.5f };
		//cout << "sizeParamX: " << sizeParamObj.x << "sizeParamY: " << sizeParamObj.y << endl;

		auto [leftBounder, rightBounder] = calcBoundsCyclic(leftUpPos.x, sizeParamObj.x, segmentSize.x, segmentCol);
		auto [topBounder, bottomBounder] = calcBoundsCyclic(leftUpPos.y, sizeParamObj.y, segmentSize.y, segmentRows);
		/*cout << "left: " << leftBounder << "right: " << rightBounder << "top: " << topBounder << "bott: " << bottomBounder << endl;*/

		return
		{
			static_cast<short>(leftBounder),
			static_cast<short>(rightBounder),
			static_cast<short>(topBounder),
			static_cast<short>(bottomBounder)
		};
	}

	bool Map::isInterval(const short firstInd, const short secondInd, const segmentInterval& interval, const Vector2f& sizeParamObj)
	{
		bool gapX = interval.left >= interval.right && sizeParamObj.x > segmentSize.x;
		bool gapY = interval.top >= interval.bottom && sizeParamObj.y > segmentSize.y;

		bool isX = gapX ? (firstInd >= interval.left || firstInd <= interval.right)
			: (firstInd >= interval.left && firstInd <= interval.right);

		bool isY = gapY ? (secondInd >= interval.top || secondInd <= interval.bottom)
			: (secondInd >= interval.top && secondInd <= interval.bottom);
		return isX && isY;
	}

	std::pair<int, int> Map::calcBoundsCyclic(float pos, float sizeParamObj, float segmentSize, int segCount) const
	{
		int left = static_cast<int>(floor(pos / segmentSize));
		int right = static_cast<int>(floor((pos + sizeParamObj) / segmentSize));

		left = normalize(left, segCount);
		right = normalize(right, segCount);

		return { left, right };
	}

	int Map::normalize(int ind, int segCount) const
	{
		int res = ind % segCount;
		return (res < 0) ? res + segCount : res;
	}


	const MapSegment& Map::getSegment(sf::Vector2i indices) const
	{
		if (isIndicesOut(indices))
		{
			cerr << "indices out of Segment\n";
			throw std::out_of_range("indices out of Segment");
		}
		return m_mapSegments[indices.x][indices.y];
	}

	MapSegment& Map::getSegment(sf::Vector2i indices)
	{
		if (isIndicesOut(indices))
		{
			cerr << "indices out of Segment\n";
			throw std::out_of_range("indices out of Segment");
		}
		return m_mapSegments[indices.x][indices.y];
	}

	bool Map::isIndicesOut(const sf::Vector2i indices) const
	{
		return (indices.x < 0 || indices.y < 0 || indices.x >= segmentCol || indices.y >= segmentRows);
	}

}