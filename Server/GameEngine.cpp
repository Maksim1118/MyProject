#include "GameEngine.h"
#include "BaseOld.h"
#include "HeroServer.h"
#include "MessageServer.h"
#include "PieceServer.h"
#include "Generator.h"
#include "Constants.h"

#include <ctime>
#include <algorithm>
#include<iostream>
#include <unordered_set>
#include <numeric>

namespace Server
{

	enum class States;
	void runServer(GameEngine& _Engine)
	{
		Clock clock;
		Time time = clock.getElapsedTime();
		while (1)
		{
			try
			{
				Time time2 = clock.getElapsedTime();
				int  diff = time2.asMilliseconds() - time.asMilliseconds();
				time = time2;
				_Engine.TimeElapsed(diff);
				lock_guard<mutex> lock(m);
 				if (!messageServer.isResponse)
				{
					messageServer.response = _Engine.process(messageServer.request);
					messageServer.isResponse = true;
				}
			}
			catch (const std::exception& e)
			{
				cerr << "Error runServer: " << e.what() << endl;
			}
		}
	}

	GameEngine::GameEngine()
		:m_camera({ fieldWidth, fieldHeight })
	{
		m_map = make_unique<Map>();
		m_map->createSceneObjects();
		srand(time(NULL));
	}

	nlohmann::json GameEngine::HeroToJson(Hero& hero)
	{
		nlohmann::json Hero;
		auto heroCol = static_cast<int>(hero.colB);
		Hero["id"] = hero.getID();
		Hero["Center"] = { {"x", hero.getCenter().x},{ "y", hero.getCenter().y} };
		
		Hero["Radius"] = hero.getRadius();
		Hero["Color"] = heroCol;
		Hero["Name"] = hero.getName();
		Hero["Splitted"] = hero.isSplitting;
		Hero["Mass"] = hero.getMass(); 
		Hero["MaxV"] = hero.getMaxV();
		Hero["GameOver"] = hero.isGameOver();
		Hero["Speed"]["x"] = hero.getSpeed().x;
		Hero["Speed"]["y"] = hero.getSpeed().y;
		
		Hero["Type"] = hero.getType();
	
		return Hero;
	}

	void GameEngine::TimeElapsed(int& diff)
	{
		m_map->TimeElapsed(diff);
		
	}

	nlohmann::json GameEngine::process(nlohmann::json request)
	{
		nlohmann::json response;
		if (request.contains("action") && request["action"] == "create Player")
		{
			const string name = request["Name"].get<string>();
			shared_ptr<Hero> hero = make_shared<Hero>(m_map.get(), name);
			if (hero->registerSelf())
			{
				m_map->add(hero);
			}

			response["Player"] = HeroToJson(*hero);

			response["status"] = "OK";
		}
		else if (request.contains("action") && request["action"] == "split")
		{
			string id = request["Id"];
			auto obj = m_map->findObj(id);
			if (obj)
			{
				std::shared_ptr<Hero> heroPtr = std::dynamic_pointer_cast<Hero>(obj);
				heroPtr->split();
				response["status"] = "OK";
			}

		}
		else if (request.contains("action") && request["action"] == "feed")
		{
			string id = request["Id"];
			auto obj = m_map->findObj(id);
			if (obj)
			{
				std::shared_ptr<Hero> heroPtr = std::dynamic_pointer_cast<Hero>(obj);
				//to do feed;
				response["status"] = "OK";
			}
		}

		else if (request.contains("action") && request["action"] == "ask pos mouse")
		{
			string id = request["Id"];
			auto obj = m_map->findObj(id);
			if (obj)
			{
				std::shared_ptr<Hero> heroPtr = std::dynamic_pointer_cast<Hero>(obj);
				float x = request["mousePosition"]["x"];
				float y = request["mousePosition"]["y"];

				heroPtr->setPosMouse(x, y);
				response["status"] = "OK";
			}
		}

		else if (request.contains("action") && request["action"] == "get Map Info")
		{
			response["mapSize"]["x"] = MapConstants::mapWidth;
			response["mapSize"]["y"] = MapConstants::mapHeight;

			response["status"] = "OK";
		}

		else if (request.contains("action") && request["action"] == "get Segment Info")
		{
			/*response["segmentSize"]["x"] = m_segmentSize.x;
			response["segmentSize"]["y"] = m_segmentSize.y;

			response["maxIndices"]["x"] = std::size(m_segments[0]);
			response["maxIndices"]["y"] = std::size(m_segments);*/
		}
		else if (request.contains("action") && request["action"] == "get object")
		{
			string id = request["Id"].get<string>();
			auto obj = m_map->findObj(id);
			std::shared_ptr<Hero> player = std::dynamic_pointer_cast<Hero>(obj);
			if (player)
			{
				auto& listObjects = m_map->getNearObjects(FloatRect(0, 0, 2490, 2490));
				nlohmann::json jsonArr = nlohmann::json::array();

				for (auto& item : listObjects)
				{
					switch (item->getType())
					{
					case ObjectType::FOOD:
						jsonArr.push_back(FoodToJson(*dynamic_pointer_cast<Food>(item)));
						break;
					case ObjectType::THORN:
						jsonArr.push_back(ThornToJson(*dynamic_pointer_cast<Thorn>(item)));
						break;
					case ObjectType::PIECE:
						jsonArr.push_back(PieceToJson(*dynamic_pointer_cast<Piece>(item)));
						break;
					case ObjectType::PLAYER:
						jsonArr.push_back(HeroToJson(*dynamic_pointer_cast<Hero>(item)));
						break;
					case ObjectType::BOT:
						jsonArr.push_back(BotToJson(*dynamic_pointer_cast<Bot>(item)));
						break;
					default:
						break;
					}
				}
				if (player->isSplitting)
				{
					jsonArr.push_back(HeroToJson(*dynamic_pointer_cast<Hero>(player)));
				}
				response["listObjects"] = jsonArr;
				response["status"] = "OK";
			}
		}
		else
		{
			response["status"] = "ERROR";
		}
		return response;
	}

	nlohmann::json GameEngine::BotToJson(Bot& bot)
	{
		auto botCol = static_cast<int>(bot.colB);
		nlohmann::json Bot;
		Bot["id"] = bot.getID();
		Bot["Center"] = { {"x", bot.getCenter().x},{ "y", bot.getCenter().y} };
		Bot["Radius"] = bot.getRadius();
		Bot["Mass"] = bot.getMass();
		Bot["Color"] = botCol;
		Bot["Name"] = bot.getName();
		Bot["Speed"]["x"] = bot.getSpeed().x;
		Bot["Speed"]["y"] = bot.getSpeed().y;
		Bot["Type"] = bot.getType();
		
		return Bot;
	}

	nlohmann::json GameEngine::PieceToJson(Piece& piece)
	{
		nlohmann::json Piece;
		Piece["id"] = piece.getID();
		Piece["Center"] = { {"x", piece.getCenter().x},{ "y", piece.getCenter().y} };
		Piece["Radius"] = piece.getRadius();
		Piece["ParentId"] = piece.getParentID();
		Piece["Mass"] = piece.getMass();
		Piece["Color"] = piece.m_ParentColor;
		Piece["Speed"]["x"] = piece.getSpeed().x;
		Piece["Speed"]["y"] = piece.getSpeed().y;
		Piece["Type"] = piece.getType();
		Piece["MaxV"] = piece.getMaxV();
		return Piece;
	}

	nlohmann::json GameEngine::FoodToJson(Food& food)
	{
		nlohmann::json Food;
		Food["Center"] = { {"x", food.getCenter().x},{ "y", food.getCenter().y} };
		Food["Radius"] = food.getRadius();
		Food["Color"] = colorToJson(food.getColor());
		Food["id"] = food.getID();
		Food["Type"] = food.getType();
		nlohmann::json indicesJson = nlohmann::json::array();
		
		return Food;
	}

	nlohmann::json GameEngine::FeedToJson(Feed& feed)
	{
		nlohmann::json Feed;
		Feed["Center"] = { {"x", feed.getCenter().x},{ "y", feed.getCenter().y} };
		Feed["Radius"] = feed.getRadius();
		Feed["id"] = feed.getID();
		Feed["Speed"]["x"] = feed.getSpeed().x;
		Feed["Speed"]["y"] = feed.getSpeed().y;
		Feed["Type"] = feed.getType();
		return Feed;
	}

	nlohmann::json GameEngine::ThornToJson(Thorn& thorn)
	{
		nlohmann::json Thorn;
		Thorn["Center"] = { {"x", thorn.getCenter().x},{ "y", thorn.getCenter().y} };
		Thorn["Radius"] = thorn.getRadius();
		Thorn["id"] = thorn.getID();
		Thorn["Type"] = thorn.getType();
		return Thorn;
	}

	nlohmann::json GameEngine::colorToJson(sf::Color color)
	{
		return nlohmann::json
		{
			{"r", color.r}, {"g", color.g}, {"b", color.b}, {"a", color.a}
		};
	}
}
