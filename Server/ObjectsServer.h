#pragma once
#include "BaseOld.h"
#include "nlohmann\json.hpp"

#include <string>
#include <list>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
namespace Server {

	class MoveObject;
	enum class States
	{
		LIVE, EATEN, READY_TO_LIVE
	};

	class Objects
	{
	public:
		float _mass{0};
		Objects(Vector2f center, float mass);
		virtual ~Objects();
		void setCenter(float& x, float& y);
		void setCenter(Vector2f& center);
		Vector2f getCenter() const;
		int getID();
		float getRadius()const;
		float getMass()const;
		void addMass(float& diff);
		bool isCollision(const Objects& obj, const float shift = 0);
		virtual void TimeElapsed(int diff) = 0;
		string  getDescription();
		bool Eating(Objects& obj, float shift);
		virtual bool checkEaten(MoveObject* other) = 0;
		States state;
	protected:
		int id;
		Vector2f _center{ 0,0 };
	private:
		static int lastId;
	};


}
