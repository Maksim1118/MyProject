#pragma once
#include "nlohmann\json.hpp"
#include "ObjectsServer.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
namespace Server 
{
	class MoveObject: public virtual Objects
	{
	public:
		MoveObject(Vector2f center, float mass);
		void Move(Vector2f& diff);
		bool readyToWaekend = false;
		Vector2f getSpeed();
	protected:
		Vector2f V;
		Vector2f _Mouse;
	};
}