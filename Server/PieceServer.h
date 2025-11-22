#pragma once
#include "MoveObjectServer.h"
#include "ObjectsServer.h"


namespace Server
{
	class Bot;


	enum class PieceState
	{
		None, Merge, Phantom
	};

	class Piece : public MoveObject
	{
	public:
		Piece(IRegistrator* iRegistrator, Bot* bot, float mass, const sf::Vector2f& center, const sf::Vector2f& v,PieceState ability = PieceState::None);
		bool checkEaten(Objects& eatingObj) override;
		bool Eat(Objects& obj) override;
		void TimeElapsed(int diff) override;
		void setTarget(const sf::Vector2f& targetP);
		void hungry(int diff);
		PieceState getAbility() const;
		const std::string& getParentID() const;
		void setTimerSpeed();
		int m_ParentColor;

		static constexpr float koefMax_V = 3.5f;
		static constexpr int PHANTOM_DUR = 1000;
		static constexpr int EXCLUSION_MERGE_DUR = 5000;
		static constexpr int TIMER_SPEED = 2500;
	private:
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
		void initTimerAbility();
		void updateAbility(int diff);
		Bot* m_parent;
		sf::Vector2f target;
		PieceState ability;
		int timerActiveAbility;
		int timerActiveSpeed;
	};

}

