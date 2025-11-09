#pragma once
#include "nlohmann\json.hpp"
#include "ObjectsServer.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
namespace Server 
{
	class MoveObject: public Objects
	{
	public:
		MoveObject(IRegistrator* iRegistrator, const Vector2f& center, float mass, const sf::Vector2f& v);
		void Move(Vector2f& diff);
		bool readyToWaekend = false;
		Vector2f getSpeed();
		sf::Vector2f getDirection() const;
		const float getMaxV() const;
	protected:
		Vector2f V;
		sf::Vector2f m_dir;
		Vector2f _Mouse;
		float maxV;
	};
}