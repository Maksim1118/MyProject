#pragma once
#include "FoodServer.h"
#include "HeroServer.h"
#include "ThornServer.h"
#include "nlohmann\json.hpp"

#include <unordered_map>

namespace  Server
{
	constexpr auto ArrFoodSize = 500;
	constexpr int SetPosFood = 1190;
	constexpr int SetPosThorn = 1100;
	class SceneManager;
	class GameEngine
	{
	public:
		GameEngine();
		nlohmann::json process(nlohmann::json request);
		bool isCollWithMap(const Vector2f& pos);
		Vector2f getCoorCollWithMap(const  Vector2f& pos);
		Vector2f getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height);
		virtual void TimeElapsed(int& diff);

		bool _GameOver = false;
		bool thornEaten = false;

		list<shared_ptr<Thorn>> _ThornSprite{};
		list<shared_ptr<Food>> _Food{};

		Hero hero;
		list<shared_ptr<Bot>> bots{};
	private:
		list<shared_ptr<Feed>> m_ListFeeds;
		nlohmann::json getInformationHero(Hero& hero);
		template<typename T> void shift(T* ptrObj);
		template<typename T> void fillList(list<shared_ptr<T>>& list, size_t size, string str);
		void allObjectsCollWithMap();
		void calcDirMoveBot(int diff);
		Vector2f calcDistBotAndObj(Objects& obj1, Objects& obj2, float zeroZone);
		template<typename T> void updatePosAfterEaten(shared_ptr<T>& ptrObj, list< shared_ptr<T>>& listPtrObjects);
		template<typename T> bool checkCollision(Objects * obj, list<shared_ptr<T>>& _list);
		template<typename T> void  spawnList(list<shared_ptr<T>>& _list);
		bool checkAllCollision(Objects& obj);
		template<typename T> void checkStatus(list<shared_ptr<T>>& _list, int diff);
	};
	void runServer(GameEngine& _Engine);
}

