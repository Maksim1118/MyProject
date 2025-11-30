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
		sf::FloatRect getMBR()const override;
		//temp var 
		bool isSplitting = false;
		void TimeElapsed(int diff)override;
		void split();
		void createTwoSplitPiece(const float origMass, const sf::Vector2f& origCenter, const sf::Vector2f& origV, sf::Vector2f& dir);
		void setPosMouse(float x, float y);
		std::string getName();
		float getMinRadius() const;
		Vector2f getMouse();
		BotsColor colB;
	protected:
		void hungry(int diff);
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


