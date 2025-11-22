#pragma once
#include "FoodServer.h"
#include "HeroServer.h"
#include "ThornServer.h"
#include "nlohmann\json.hpp"
#include "Map.h"
#include "QuadTree.h"
#include "RTree.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>


namespace  Server
{
	constexpr auto ArrFoodSize = 500;
	constexpr int SetPosFood = 1190;
	constexpr int SetPosThorn = 1100;

	constexpr float timeRespawnFood = 200.f;
	constexpr float timeRespawnThorn = 3000.f;
	constexpr float timeRespawnBot = 10000.f;

	constexpr float offsetSpawn = 2.f;

	constexpr float EPSILON = 0.001f;

	constexpr int maxAttempsSpawn = 100;
	constexpr float densePercent = 0.4f;

	class SceneManager;
	class GameEngine
	{
	public:
		GameEngine();
		virtual void TimeElapsed(int& diff);
		nlohmann::json process(nlohmann::json request);
	private:
		std::unique_ptr<Map> m_map;
		Camera m_camera;
	private:
		static nlohmann::json HeroToJson(Hero& hero);
		static nlohmann::json BotToJson(Bot& bot);
		static nlohmann::json PieceToJson(Piece& piece);
		static nlohmann::json FoodToJson(Food& food);
		static nlohmann::json FeedToJson(Feed& feed);
		static nlohmann::json ThornToJson(Thorn& thorn);
		static nlohmann::json colorToJson(sf::Color color);

		template<typename Container, typename Func, typename PtrGetter>
		static void fillListToJson(Container& list, Func convertFunc, PtrGetter getPtr, nlohmann::json& outJsonArray)
		{
			
			for (auto& element : list)
			{
				auto* objPtr = getPtr(element);
				auto jsonObj = convertFunc(*objPtr);

				outJsonArray.push_back(jsonObj);
			}
			
		}
		
	};
	void runServer(GameEngine& _Engine);
}

