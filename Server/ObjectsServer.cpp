#include "ObjectsServer.h"


#include "IRegistrator.h"
#include "Generator.h"

#include "UniqueIdGenerator.h"
#include "Constants.h"

#include <math.h>

namespace Server {
	Objects::UnRegisterFunc Objects::unRegFunc = nullptr;

	void Objects::setUnRegisterFunc(UnRegisterFunc func)
	{
		if (func)
		{
			unRegFunc = func;
		}
	}

	Objects::Objects(IRegistrator* iRegistrator, Vector2f center, float mass) : registrator(iRegistrator), _center(center), _mass(mass), id(UniqueIdGenerator::generateID())
	{
		checkRegistrator(registrator);
		initOffsetSentTime();
		active = true;
	}

	bool Objects::registerSelf()
	{
		if (!registrator->spawn(getRadius(), _center))
		{
			return false;
		}

		auto ptr = shared_from_this();
		registrator->registerAuxiliary(ptr);
	}

	bool Objects::isActive()
	{
		return active;
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

	void Objects::setLive()
	{
		m_isLive = true;
	}

	Vector2f Objects::getCenter()  const
	{
		return _center;
	}

	sf::FloatRect Objects::getBounds() const
	{
		return sf::FloatRect( _center.x - getRadius(), _center.y - getRadius(), getRadius() * 2.f, getRadius() * 2.f);
	}
	const std::string& Objects::getID() const
	{
		return id;
	}

	float Objects::getRadius() const
	{
		return sqrt(_mass);
	}

	const float Objects::getMass() const
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
			_mass += obj.getMass() * 6.f;
			return true;
		}
		return false;
	}


	void Objects::unRegister()
	{
		if (unRegFunc)
		{
			unRegFunc(this->getID());
		}
	}

	/*nlohmann::json Objects::toJson()
	{
		nlohmann::json res;
		res["type"] = type;
		auto staticJs = toStaticJson();
		if (staticJs != lastSnapshot || staticSentDataTime >= ServerConstants::TIMER_SENT_STATIC_DATA)
		{
			res["staticData"] = staticJs;
			staticSentDataTime = 0;
		}

		res["persistentData"] = toPersistentJson();
		return res;
	}*/

	void Objects::initOffsetSentTime()
	{
		staticSentDataTime = genNumber<int>(-1, ServerConstants::TIMER_SENT_STATIC_DATA - 1);
	}

	bool Objects::isLive()
	{
		return m_isLive;
	}

	ObjectType Objects::getType() const
	{
		return type;
	}

	ObjectState Objects::getState() const
	{
		return m_state;
	}

	bool Objects::isCollision(const Objects& obj, const float shift)
	{
		Vector2f Pos1 = getCenter();
		Vector2f Pos2 = obj.getCenter();
		return GetCyclicDist(Pos1, Pos2, MapConstants::mapWidth, MapConstants::mapHeight) < getRadius() + obj.getRadius() + shift;
	}

	string  Objects::getDescription()
	{
		return  (string)"( x = "  + to_string(getCenter().x) + " , y = " + to_string(getCenter().y) + " , mass = " + to_string(getMass()) + " )";
	}
}