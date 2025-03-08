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
	Bot(Vector2f center, float radius, int Col, string text, int id);
	void TimeElapsed(int diff);
	void isSplitted(bool splite);
	void setPosMouse(float x, float y);
	void draw(RenderWindow& window)override;
	list<shared_ptr<Piece>> pieces;
protected:
	void pieceToSides();
	CircleShape _shape;
	Text name;
	bool splitted;	
};