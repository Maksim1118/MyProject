#pragma once
#include <SFML/Graphics.hpp>
#include "Objects.h"
#include "MoveObject.h"
#include "Piece.h"

#include <list>
#include <memory>

class Bot : public MoveObject
{
public:
	Bot(Vector2f center, float radius, int Col, string text, string id);
	void TimeElapsed(int diff) override;
	void setSplitted(bool splitted);
	bool isSplitted() const;
	Vector2f getDirection() const;
	void draw(RenderWindow& window) const override;
	void setCenter(Vector2f newCenter) override;
	void shiftPos(float offsetX = 0, float offsetY = 0) override;
	list<Piece*> pieces;
protected:
	/*void pieceToSides();*/
	CircleShape _shape;
	Text name;
	bool m_Splitted = false;	
};