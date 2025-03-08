#pragma once
#include "ObjectsServer.h"
#include "PieceServer.h"
#include "MoveObjectServer.h"
#include <list>
#include "FoodServer.h"
#include <iostream>
#include <memory>
namespace Server
{
	enum class BotsColor
	{
		BotsYellow, BotsRed, botsBlue, BotsGreen, BotsBrown, BotsPurple
	};

	class Bot : public MoveObject
	{
	public:
		Bot(string  text);
		Bot& operator =(const Bot&);

		void TimeElapsed(int diff)override;
		void setWeakened();
		void setSplite();
		void setPosMouse(float x, float y);
		bool checkEaten(MoveObject* other) override;
		string getName();
		Vector2f getMouse();
		bool isSplitted();
		void Eat(Objects* obj);
		BotsColor colB;
		list<shared_ptr<Piece>> pieces;
	protected:
		bool createPiece(float& mass, Vector2f& center);
		void together();
		void pieceToSides();
		Vector2f calcRepulsiveForce(shared_ptr<Piece>& piece);
		Vector2f calcAcceleration(Vector2f F, shared_ptr<Piece>& piece);
		Vector2f calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece);
		void hungry(int diff);
		void udateMass();
		bool setPieceWeakend(list<shared_ptr<Piece>>::iterator& obj);
		string name;
		int prevMass;
		bool Splitted;
		int Timer{ 0 };
		int count{ 0 };
		int curTime;
		int respawnTime;
	private:
		void update(int diff);

	};
}


