#pragma once
#include "Objects.h"
#include "MoveObject.h"

extern  Color botsArrColor[];

class Piece : public MoveObject
{
public:
	Piece(Vector2f center, float  radius, int Col, std::string id, const std::string& parentId);
	void TimeElapsed(int diff) override;
	void draw(RenderWindow& window) const override;
	void shiftPos(float offsetX = 0, float offsetY = 0) override;
	Vector2f getCenter();
	Vector2f getSpeed();
	void setExclude(bool exclude);
	bool isExclude() const;

	inline const std::string& getParentID() const
	{
		return m_parentId;
	}

	void setCenter(Vector2f newCenter) override;
private:
	bool m_isExcluded;
	std::string m_parentId;
	CircleShape _shape;

};