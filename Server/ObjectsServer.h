#pragma once

#include "BaseOld.h"
#include "nlohmann\json.hpp"

#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include <functional>

using namespace sf;
using namespace std;
namespace Server {

	class IRegistrator;
	class MoveObject;

	enum class ObjectType
	{
		FOOD, THORN, BOT, PLAYER, PIECE, FEED
	};

	enum class ObjectState
	{
		Respawnable, Delitable
	};

	enum class States
	{
		LIVE, EATEN, READY_TO_REMOVE, READY_TO_RESPAWN, READY_TO_LIVE
	};

	class I_MBR
	{
	public:
		virtual sf::FloatRect getMBR()const = 0;
	};

	class Objects: public std::enable_shared_from_this<Objects>, public I_MBR
	{
	public:
		Objects(IRegistrator* iRegistrator, Vector2f center, float mass);
		bool registerSelf();
		virtual bool Eat(Objects& obj) = 0;
		bool isActive();
		virtual bool checkEaten(Objects& eatingObj) = 0;
		sf::FloatRect getMBR()const override = 0;

		using UnRegisterFunc = std::function<void(const std::string&)>;

		static void setUnRegisterFunc(UnRegisterFunc func);
		float _mass{0};
		virtual ~Objects();
		void setCenter(float& x, float& y);
		void setCenter(Vector2f& center);
		void setLive();
		Vector2f getCenter() const;
		const std::string & getID() const;
		float getRadius()const;
		const float getMass()const;
		void addMass(float& diff);
		bool isCollision(const Objects& obj, const float shift = 0);
		virtual void TimeElapsed(int diff) = 0;
		string  getDescription();
		bool Eating(Objects& obj, float shift);
		bool isLive();
		sf::Vector2i m_currSegIndices;
		ObjectType getType() const;
		ObjectState getState() const;
		void unRegister();
	protected:
		virtual nlohmann::json toStaticJson() const = 0;
		virtual nlohmann::json toPersistentJson() const = 0;
	protected:
		int staticSentDataTime;
		nlohmann::json lastSnapshot;

		IRegistrator* registrator;
		bool active; 
		Vector2f _center{ 0,0 };
		ObjectType type;
		ObjectState m_state;
		bool m_isLive;

		//только для статических объектов
		mutable FloatRect localBounds;
	private:
		void initOffsetSentTime();
	private:
		static UnRegisterFunc unRegFunc;
		const std::string id;
	};
}
