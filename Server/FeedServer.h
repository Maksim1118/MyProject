#pragma once
#include "FoodServer.h"
#include "MoveObjectServer.h"

namespace Server
{
	class Feed : public MoveObject
	{
	public:
		Feed(IRegistrator* iRegistrator);
		bool Eat(Objects& obj) override;
		bool checkEaten(Objects& eatingObj) override;
		void TimeElapsed(int diff);
		void setV(Vector2f& newV);
		void setParentCenter(Vector2f& newCenter);
		void setParentRadius(float newRadius);
	/*	void setEatenState() override;*/
	private:
		nlohmann::json toStaticJson() const override;
		nlohmann::json toPersistentJson() const override;
	private:
		Vector2f parentCenter;
		float parentRadius;
		int colorNum;
		int respawnTime;
		int curTime;
		bool m_IsUnderHero;
	};
}


