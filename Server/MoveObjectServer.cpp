#include "MoveObjectServer.h"

#include "IRegistrator.h"

#include "Constants.h"

#include "BaseOld.h"

namespace Server
{

	MoveObject::MoveObject(IRegistrator* iRegistrator, const Vector2f& center, float mass, const sf::Vector2f& v) : Objects(iRegistrator, center, mass), V(v), m_dir(0.f, 0.f), _Mouse(0.f, 0.f), maxV(0.f) {}

	void MoveObject::Move(Vector2f& diff)
	{
		_center += diff;
		_center.x = wrapCoordinate(_center.x, MapConstants::mapWidth);
		_center.y = wrapCoordinate(_center.y, MapConstants::mapHeight);
	}

	Vector2f MoveObject::getSpeed()
	{
		return V;
	}
	sf::Vector2f MoveObject::getDirection() const
	{
		return m_dir;
	}
	const float MoveObject::getMaxV() const
	{
		return maxV;
	}
}
