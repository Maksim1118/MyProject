#pragma once
#include "ObjectsServer.h"
#include "MoveObjectServer.h"
#include <list>
#include "FoodServer.h"
#include "PieceServer.h"
#include "Respawn.h"
#include <iostream>
#include <memory>

namespace Server
{
	constexpr float minBotAndPlayerMass = 400.f;

	enum class BotsColor
	{
		BotsYellow, BotsRed, botsBlue, BotsGreen, BotsBrown, BotsPurple
	};

	class PiecesList;

	class Bot : public MoveObject, public Respawn
	{
	public:
		Bot(IRegistrator* iRegistrator);
		bool Eat(Objects& obj) override;
		bool checkEaten(Objects& eatingObj) override;
		virtual void removePiece(const std::string& id);
		//temp var 
		bool isSplitting = false;

		/*Bot& operator =(const Bot&);*/

		void TimeElapsed(int diff)override;
		/*void setWeakened();*/
		void split();
		void createTwoSplitPiece(const float origMass, const sf::Vector2f& origCenter, const sf::Vector2f& origV, sf::Vector2f& dir);
		void setPosMouse(float x, float y);
	/*	void setEatenState() override;
		void removePiece(const string& id);*/

		std::string getName();
		float getMinRadius() const;
		Vector2f getMouse();
		/*bool isSplitted();*/
		BotsColor colB;
	protected:
		void hungry(int diff);

		//bool createPiece(float& mass, Vector2f& center);
		//float getMinDist(float radius);
		//void updatePieceMove(std::shared_ptr<Piece>& piece, sf::Vector2f& v, int diff);
		//bool canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2);
		//void mergePieces(int diff);

	/*	void together();*/
		/*void pieceToSides();*/
		//Vector2f calcAttractionForce(shared_ptr<Piece>& piece);
		//sf::Vector2f calcAttractionForce(sf::Vector2f& dir, float koef = 1.f);
		//sf::Vector2f calcAttractionForceToMouse(std::shared_ptr<Piece>& piece);
		//Vector2f calcAcceleration(Vector2f F, float mass);
		//Vector2f calcSpeed(Vector2f a, int diff, shared_ptr<Piece>& piece);
		
		//void udateMass();
	/*	bool setPieceWeakend(shared_ptr<Piece>& obj);*/

	/*	void updateSplitTimer(float diff);
		void updatePiecePhysic(float diff, shared_ptr<Piece>& piece);
		void updateMove(float diff);
		void clearListPieces();
		void reset();*/
	protected:
		std::unique_ptr<PiecesList> m_piecesList;

		std::string m_Name;
		int prevMass;
		bool Splitted;
		int count{ 0 };
	private:
		Vector2f calcTargetPoint() const;
		void respawn() override;
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
	};
}


