#pragma once
#include "ObjectsServer.h"
#include "MoveObjectServer.h"
#include <list>
#include "FoodServer.h"
#include "PieceServer.h"
#include <iostream>
#include <memory>

namespace Server
{
	constexpr float minBotAndPlayerMass = 400.f;

	enum class BotsColor
	{
		BotsYellow, BotsRed, botsBlue, BotsGreen, BotsBrown, BotsPurple
	};

	class Bot : public MoveObject
	{
	public:
		Bot();
		Bot& operator =(const Bot&);

		void TimeElapsed(int diff)override;
		void setWeakened();
		void setSplite();
		void setPosMouse(float x, float y);
		void setEatenState() override;

		bool checkEaten(MoveObject* other) override;
		void eatPiece(const string& id);

		string getName();
		float getMinRadius() const;
		Vector2f getMouse();
		bool isSplitted();
		void Eat(Objects* obj);
		BotsColor colB;
		std::unordered_map<std::string, std::shared_ptr<Piece>> pieces;
	protected:
		bool createPiece(float& mass, Vector2f& center);
		float getMinDist(float radius);
		void updatePieceMove(std::shared_ptr<Piece>& piece, sf::Vector2f& v, int diff);
		bool canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2);
		void mergePieces(int diff);

		void together();
		void pieceToSides();
		//Vector2f calcAttractionForce(shared_ptr<Piece>& piece);
		sf::Vector2f calcAttractionForce(sf::Vector2f& dir, float koef = 1.f);
		sf::Vector2f calcAttractionForceToMouse(std::shared_ptr<Piece>& piece);
		Vector2f calcAcceleration(Vector2f F, float mass);
		Vector2f calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece);
		void hungry(int diff);
		void udateMass();
		bool setPieceWeakend(shared_ptr<Piece>& obj);

		void updateSplitTimer(float diff);
		void updatePiecePhysic(float diff, shared_ptr<Piece>& piece);
		void updateMove(float diff);
		void clearListPieces();
		void reset();
	protected:
		string m_Name;
		int prevMass;
		bool Splitted;
		int Timer;
		int count{ 0 };
		int curTime;
		int elapsedRespTime;
	private:
		void update(int diff);

	};
}


