#include "ObjectsServer.h"

#include "UniqueIdGenerator.h"

#include <math.h>

namespace Server {

	Objects::Objects(Vector2f center, float mass) : _center(center), _mass(mass), m_currSegIndices(-10, -10)
	{
		id = UniqueIdGenerator::generateID();
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

	sf::FloatRect Objects::getBounds() const
	{
		return sf::FloatRect( _center.x - getRadius(), _center.y - getRadius(), getRadius() * 2.f, getRadius() * 2.f);
	}

	string Objects::getID()
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
			_mass += obj.getMass() * 3.f;
			return true;
		}
		return false;
	}

	bool Objects::isLive()
	{
		return state == States::LIVE;
	}

	bool Objects::isCollision(const Objects& obj, const float shift)
	{
		Vector2f Pos1 = getCenter();
		Vector2f Pos2 = obj.getCenter();
		return GetDist(Pos1, Pos2) < getRadius() + obj.getRadius() + shift;
	}

	string  Objects::getDescription()
	{
		return  (string)"( x = "  + to_string(getCenter().x) + " , y = " + to_string(getCenter().y) + " , mass = " + to_string(getMass()) + " )";
	}
}