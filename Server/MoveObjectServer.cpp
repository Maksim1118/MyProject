#include "MoveObjectServer.h"

namespace Server
{
	MoveObject::MoveObject(Vector2f center, float mass) : Objects(center, mass), V(0.f, 0.f), _Mouse(0.f, 0.f) {}

	void MoveObject::Move(Vector2f& diff)
	{
		_center += diff;
	}

	Vector2f MoveObject::getSpeed()
	{
		return V;
	}
}
