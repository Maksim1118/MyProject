#pragma once
#include "MoveObjectServer.h"
#include "ObjectsServer.h"


namespace Server
{
	class Bot;

	class Piece : public MoveObject
	{
	public:
		Piece(Bot* bot = nullptr);
		void TimeElapsed(int diff);
		void setV(Vector2f newV);
		void setMaxV(float newMaxV);
		bool checkEaten(MoveObject* other)override;
		void setEatenState() override;
		bool isExcludedFromMerge() const;
		void setExcludedFlag(const bool exclude);
		float getMaxV();
		Bot* getParentPointer();
		void Eat(Objects* obj);
		int m_ParentColor;
	private:
		void updateTimerActive(const int diff);
		Bot* m_BotParent = nullptr;
		int timerActive;
		bool excludedFromMerge;
		float maxV;
	};

}

