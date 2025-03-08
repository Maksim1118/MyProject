#include "ObjectsServer.h"

#include <math.h>

namespace Server {

	int Objects::lastId = 0;

	Objects::Objects(Vector2f center, float mass) : _center(center), id(lastId++), _mass(mass)
	{
		state = States::LIVE;
	}

	Objects::~Objects() = default;

	void Objects::setCenter(float& x, float& y)
	{
		_center.x = x;
		_center.y = y;
	}

	void Objects::setCenter(Vector2f& center)
	{
		_center = center;
	}

	Vector2f Objects::getCenter()  const
	{
		return _center;
	}

	int Objects::getID()
	{
		return id;
	}

	float Objects::getRadius() const
	{
		return sqrt(_mass);
	}

	float Objects::getMass()const
	{
		return  _mass;
	}

	void Objects::addMass(float& diff)
	{
		_mass += diff;
	}

	bool Objects::Eating(Objects& obj, float shift)
	{
		if (isCollision(obj, shift))
		{
			_mass += obj.getMass();
			obj.state = States::EATEN;
			return true;
		}
		return false;
	}


	bool Objects::isCollision(const Objects& obj, const float shift)
	{
		Vector2f Pos1 = getCenter();
		Vector2f Pos2 = obj.getCenter();
		return GetLen(Pos1, Pos2) < getRadius() + obj.getRadius() + shift;
	}

	string  Objects::getDescription()
	{
		return  (string)"( x = "  + to_string(getCenter().x) + " , y = " + to_string(getCenter().y) + " , mass = " + to_string(getMass()) + " )";
	}
}