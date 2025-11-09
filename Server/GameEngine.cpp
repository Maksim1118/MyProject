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


//constexpr float maxMassForScale = 4000.00f;
//constexpr float minMassForScale = 400.00f;
//constexpr float maxScale = 1.00f;
//constexpr float minScale = 2.00f;

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

	/*vector<FloatRect> GameEngine::computeFreeZones(const FloatRect& mapBounds, const vector<FloatRect>& occupiedZones)
	{
		vector<FloatRect> freeZones;
		freeZones.push_back(mapBounds);

		for (const auto& occ : occupiedZones)
		{
			vector<FloatRect> newFreeZones;
			for (const auto& freeZone : freeZones)
			{
				auto part = subtractRect(freeZone, occ);
				newFreeZones.insert(newFreeZones.end(), part.begin(), part.end());
			}
			freeZones = move(newFreeZones);
		}
		return freeZones;
	}

	vector<FloatRect> GameEngine::subtractRect(const FloatRect& freeZone, const FloatRect& occ)
	{
		vector<FloatRect> result;

		if (!freeZone.intersects(occ))
		{
			result.push_back(freeZone);
			return result;
		}

		FloatRect intersection;
		freeZone.intersects(occ, intersection);

		float h, w;

		h = intersection.top - freeZone.top;
		if (h > EPSILON)
		{
			result.emplace_back(freeZone.left, freeZone.top, freeZone.width, h);
		}

		float bottomFree = freeZone.top + freeZone.height;
		float bottomInter = intersection.top + intersection.height;

		h = bottomFree - bottomInter;
		if (h > EPSILON)
		{
			result.emplace_back(freeZone.left, bottomInter, freeZone.width, h);
		}

		w = intersection.left - freeZone.left;
		if (w > EPSILON)
		{
			result.emplace_back(freeZone.left, intersection.top, w, intersection.height);
		}
		
		float rightFree = freeZone.left + freeZone.width;
		float rightInter = intersection.left + intersection.width;

		w = rightFree - rightInter;
		if (w > EPSILON)
		{
			result.emplace_back(rightInter, intersection.top, w, intersection.height);
		}

		return result;
	}*/


	/*bool GameEngine::areAdjacentOrOverlapping(const FloatRect& f1, const FloatRect& f2)
	{
		bool horizontalOverlap = (f1.left + f1.width >= f2.left - EPSILON) && (f2.left + f2.width >= f1.left - EPSILON);
		bool verticalOverlap = (f1.top + f1.height >= f2.top - EPSILON) && (f2.top + f2.height >= f1.top - EPSILON);
	
		bool adjacentHorizontally = (abs((f1.left + f1.width) - f2.left) < EPSILON) ||
			(abs((f2.left + f2.width) - f1.left) < EPSILON);
		bool adjacentVertically = (abs((f1.top + f1.height) - f2.top) < EPSILON) ||
			(abs((f2.top + f2.height) - f1.top) < EPSILON);

		return 
			(horizontalOverlap && verticalOverlap) || 
			(adjacentHorizontally && verticalOverlap) ||
			(adjacentVertically && horizontalOverlap);
	}

	FloatRect GameEngine::boundingRect(const FloatRect& f1, const FloatRect& f2)
	{
		float left = min(f1.left, f2.left);
		float right = max(f1.left + f1.width, f2.left + f2.width);
		float top = min(f1.top, f2.top);
		float bottom = max(f1.top + f1.height, f2.top + f2.height);
		return { left, top, right - left, bottom - top };
	}

	vector<FloatRect> GameEngine::mergeFreeZones(const vector<FloatRect>& zones)
	{
		vector<FloatRect> merged = zones;
		bool changed;
		do
		{
			changed = false;
			for (int i = 0; i < merged.size(); ++i)
			{
				for (int j = i + 1; j < merged.size(); ++j)
				{
					if (areAdjacentOrOverlapping(merged[i], merged[j]))
					{
						merged[i] = boundingRect(merged[i], merged[j]);
						merged.erase(merged.begin() + j);
						changed = true;
						goto endLoop;
					}
				}
			}
		endLoop:;
		} while (changed);
		return merged;
	}*/

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
		/*Hero["Feeded"] = hero.isFeeded();*/
		Hero["Mass"] = hero.getMass(); 
		Hero["MaxV"] = hero.getMaxV();
		Hero["GameOver"] = hero.isGameOver();
		Hero["Speed"]["x"] = hero.getSpeed().x;
		Hero["Speed"]["y"] = hero.getSpeed().y;
		
		Hero["Type"] = hero.getType();
	
		return Hero;
	}


	/*bool GameEngine::isCollWithMap(const  Vector2f& pos)
	{
		return (pos.x < 0) || (pos.x > fieldWidth) || (pos.y < 0) || (pos.y > fieldHeight) ? true : false;
	}*/


	/*Vector2f GameEngine::getCoorCollWithMap(const Vector2f& pos)
	{
		if (pos.x < 0)
			return Vector2f(0, pos.y);
		else if (pos.x > fieldWidth)
			return Vector2f(fieldWidth, pos.y);
		else if (pos.y < 0)
			return  Vector2f(pos.x, 0);
		else if(pos.y > fieldHeight)
			return Vector2f(pos.x, fieldHeight);
		return Vector2f(pos.x, pos.y);
	}*/

	/*void GameEngine::clearWeakList()
	{
		for (auto& it = m_WeakListPtr.begin(); it != m_WeakListPtr.end();)
		{
			if (it->expired())
			{
				it = m_WeakListPtr.erase(it);
			}
			else
			{
				++it;
			}
		}
	}*/

	//Vector2i GameEngine::findSegmentIndices(sf::Vector2f pos)
	//{
	//	int indexX = static_cast<int>(pos.x / segmentSize.x);
	//	int indexY = static_cast<int>(pos.y / segmentSize.y);
	//	return { indexX, indexY };

	//	/*res.push_back({ indexX, indexY });

	//	if (centerObj.x - (indexX * segmentSize.x) < 1e-5 && indexX > 0)
	//	{
	//		res.push_back({ indexX - 1, indexY });
	//	}
	//	if (centerObj.x - ((indexX + 1) * segmentSize.x) > -1e-5 && indexX < segmentCol - 1)
	//	{
	//		res.push_back({ indexX + 1, indexY });
	//	}
	//	if (centerObj.y - (indexY * segmentSize.y) > 1e-5 && indexY > 0)
	//	{
	//		res.push_back({ indexX, indexY - 1 });
	//	}
	//	if (centerObj.y - ((indexY + 1) *  segmentSize.y) > -1e-5 && indexY < segmentRows - 1)
	//	{
	//		res.push_back({ indexX, indexY + 1 });
	//	}

	//	return res;*/
	//	
	//}

	/*vector<Vector2i> GameEngine::objectInSegments(Vector2f center, float radius)
	{
		vector<Vector2i> res;

		Vector2f topLeft(center.x - radius, center.y - radius);
		Vector2f topRight(center.x + radius, center.y + radius);

		Vector2i start = findSegmentIndices(topLeft);
		Vector2i end = findSegmentIndices(topRight);
		
		
		start.x = clamp(start.x, 0, segmentCol - 1);
		start.y = clamp(start.y, 0, segmentRows - 1);
		end.x = clamp(end.x, 0, segmentCol - 1);
		end.y = clamp(end.y, 0, segmentRows - 1);

		for (int row = start.y; row <= end.y; ++row)
		{
			for (int col = start.x; col <= end.x; ++col)
			{
				float segX = col * segmentSize.x;
				float segY = row * segmentSize.y;

				float closestX = clamp(center.x, segX, segX + segmentSize.x);
				float closestY = clamp(center.y, segY, segY + segmentSize.y);

				float deltaX = center.x - closestX;
				float deltaY = center.y - closestY;

				if ((pow(deltaX, 2) + pow(deltaY, 2)) <= pow(radius, 2))
				{
					res.push_back({ col, row });
				}
			}
		}
		return res;
	}*/

	//bool GameEngine::isOutSegment(sf::Vector2i indices)
	//{
	//	return indices.x < 0 || indices.x >= segmentCol || indices.y < 0 || indices.y >= segmentRows;
	//}

	//void GameEngine::addInRemoveList(const std::string& id)
	//{
	//	removeList.insert(id);
	//}

	//Vector2f GameEngine::getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height)
	//{
	//	return Vector2f();
	//}

	void GameEngine::TimeElapsed(int& diff)
	{
		m_map->TimeElapsed(diff);
		/*m_TimeUpdateListObjects -= diff;
		if (m_TimeUpdateListObjects < 0.0f)
			m_TimeUpdateListObjects = 0.0f;*/


	



		/*for (auto& food : m_map.getList<Food>())
		{
			food.second->TimeElapsed(diff);
			if (food.second->m_state == States::READY_TO_LIVE)
			{
				spawnObj(food.second);
				food.second->m_state = States::LIVE;
			}
		}

		for (auto& thorn : m_map.getList<Thorn>())
		{
			thorn.second->TimeElapsed(diff);
			if (thorn.second->m_state == States::READY_TO_LIVE)
			{
				spawnObj(thorn.second);
				thorn.second->m_state = States::LIVE;
			}
		}*/

		/*for (auto& bot : m_map.getList<Bot>())
		{

			bot.second->TimeElapsed(diff);
			
			if (bot.second->m_isLive())
			{
				if(bot.second->isSplitted())
				{
					for (auto& piece : bot.second->pieces)
					{
						updateMoveObjectFromMapBounder(piece.second);
						quadTree.updateInTreePos(piece.second);
					}
				}
				else
				{
					updateMoveObjectFromMapBounder(bot.second);
					quadTree.updateInTreePos(bot.second);
				}
			}

			else if (bot.second->m_state == States::READY_TO_LIVE)
			{
				spawnObj(bot.second);
				bot.second->m_state = States::LIVE;
			}
		}

		for (auto& hero : m_map.getList<Hero>())
		{
			hero.second->TimeElapsed(diff);
			if (hero.second->m_isLive())
			{
				if (hero.second->isSplitted())
				{
					for (auto& piece : hero.second->pieces)
					{
						updateMoveObjectFromMapBounder(piece.second);
						quadTree.updateInTreePos(piece.second);
					}
				}
				else
				{
					updateMoveObjectFromMapBounder(hero.second);
					quadTree.updateInTreePos(hero.second);
				}
			}
		}

		calcDirMoveBot(diff);


		for (auto& hero : m_map.getList<Hero>())
		{
			if (hero.second->isSplitted())
			{
				for (auto& piece : hero.second->pieces)
				{
					quadTree.tryEat(piece.second);
				}
			}
			else
			{
				quadTree.tryEat(hero.second);
			}
		}

		for (auto& bot : m_map.getList<Bot>())
		{
			if (bot.second->isSplitted())
			{
				for (auto& piece : bot.second->pieces)
				{
					quadTree.tryEat(piece.second);
				}
			}
			else
			{
				quadTree.tryEat(bot.second);
			}
		}


		for (auto& hero : m_map.getList<Hero>())
		{
			if (hero.second->isSplitted())
			{
				for (auto& piece : hero.second->pieces)
				{
					quadTree.tryEat(piece.second);
				}
			}
			else
			{
				quadTree.tryEat(hero.second);
			}
		}

		for (auto& bot : m_map.getList<Bot>())
		{
			if (bot.second->isSplitted())
			{
				for (auto& piece : bot.second->pieces)
				{
					quadTree.tryEat(piece.second);
				}
			}
			else
			{
				quadTree.tryEat(bot.second);
			}
		}*/

		/*updateAndRemove(listHeroes, m_WeakListPtr, diff);
		updateAndRemove(listFeeds, m_WeakListPtr, diff);

		updateAndRemove(listFoods, m_WeakListPtr, diff);*/
		/*updateList(listFoods, diff);
		updateList(listHeroes, diff);*/

		//for (auto& food : m_map.getList<Food>())
		//{
		//	food.second->TimeElapsed(diff);
		//	if (food.second->m_isLive())
		//	{
		//		bool isUpdate = m_map.updateSegments(food.first, food.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//	else if (food.second->m_state == States::READY_TO_LIVE)
		//	{
		//		spawnObj(food.second);
		//		food.second->m_state = States::LIVE;
		//		bool isUpdate = m_map.updateSegments(food.first, food.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//}

		//for (auto& bot : m_map.getList<Bot>())
		//{
		//	bot.second->TimeElapsed(diff);

		//	if (bot.second->isSplitted())
		//	{
		//		for (auto& piece : bot.second->pieces)
		//		{
		//			updateMoveObjectFromMapBounder(piece.second);
		//			piece.second->TimeElapsed(diff);
		//			bool isUpdate = m_map.updateSegments(piece.first, piece.second);
		//			if (!isUpdate)
		//			{
		//				cerr << "error update segment\n";
		//			}
		//		}
		//		/*bool isUpdate = m_map.updateSegments(bot.first, bot.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}*/
		//	}

		//	else if(bot.second->m_isLive())
		//	{
		//		updateMoveObjectFromMapBounder(bot.second);
		//		bool isUpdate = m_map.updateSegments(bot.first, bot.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//	else if (bot.second->m_state == States::READY_TO_LIVE)
		//	{
		//		spawnObj(bot.second);
		//		bot.second->m_state = States::LIVE;
		//		bool isUpdate = m_map.updateSegments(bot.first, bot.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//}

		//for (auto& thorn : m_map.getList<Thorn>())
		//{
		//	thorn.second->TimeElapsed(diff);
		//	if (thorn.second->m_isLive())
		//	{
		//		bool isUpdate = m_map.updateSegments(thorn.first, thorn.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//	else if (thorn.second->m_state == States::READY_TO_LIVE)
		//	{
		//		spawnObj(thorn.second);
		//		thorn.second->m_state = States::LIVE;
		//		bool isUpdate = m_map.updateSegments(thorn.first, thorn.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//}

		//for (auto& hero : m_map.getList<Hero>())
		//{
		//	hero.second->TimeElapsed(diff);

		//	if (hero.second->isSplitted())
		//	{
		//		for (auto& piece : hero.second->pieces)
		//		{
		//			updateMoveObjectFromMapBounder(piece.second);
		//			piece.second->TimeElapsed(diff);
		//			bool isUpdate = m_map.updateSegments(piece.first, piece.second);
		//			if (!isUpdate)
		//			{
		//				cerr << "error update segment\n";
		//			}
		//		}
		//		/*bool isUpdate = m_map.updateSegments(hero.first, hero.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}*/
		//	}
		//	else
		//	{
		//		updateMoveObjectFromMapBounder(hero.second);
		//		bool isUpdate = m_map.updateSegments(hero.first, hero.second);
		//		if (!isUpdate)
		//		{
		//			cerr << "error update segment\n";
		//		}
		//	}
		//}

		/*clearWeakList();*/

		/*updateAndRespawn(listFoods, m_FoodTimeRespawnList, timeRespawnFood, diff);*/
	/*	updateAndRespawn(listBots, timeRespawnBot, diff);
		updateAndRespawn(listThorns, timeRespawnThorn, diff);*/

		/*allObjectsCollWithMap();*/
		

		

		/*for (int i = 0; i < std::size(m_segments); ++i)
		{
			for (int j = 0; j < std::size(m_segments[0]); ++i)
			{
				for (const auto& [key, foodPtr]: m_segments[i][j].foods)
				{
					for (const auto& [key, botPtr] : m_segments[i][j].listBots)
					{
						botPtr->Eat(foodPtr.get());
					}
				}
				for (const auto& [key, foodPtr] : m_segments[i][j].foods)
				{
					for (const auto& [key, heroPtr] : m_segments[i][j].listHeroes)
					{
						heroPtr->Eat(foodPtr.get());
					}
				}

				for (const auto& [key, thornPtr] : m_segments[i][j].thorns)
				{
					for (const auto& [key, botPtr] : m_segments[i][j].listBots)
					{
						botPtr->Eat(thornPtr.get());
					}
				}
				for (const auto& [key, thornPtr] : m_segments[i][j].thorns)
				{
					for (const auto& [key, heroPtr] : m_segments[i][j].listHeroes)
					{
						heroPtr->Eat(thornPtr.get());
					}
				}

				for (const auto& [key, botPtr1] : m_segments[i][j].listBots)
				{
					for (const auto& [key, botPtr2] : m_segments[i][j].listBots)
					{
						if (botPtr1 == botPtr2) continue;
						botPtr1->Eat(botPtr2.get());
					}
				}
				for (const auto& [key, botPtr] : m_segments[i][j].listBots)
				{
					for (const auto& [key, heroPtr] : m_segments[i][j].listHeroes)
					{
						botPtr->Eat(heroPtr.get());
						heroPtr->Eat(botPtr.get());
					}
				}
				for (const auto& [key, heroPtr1] : m_segments[i][j].listHeroes)
				{
					for (const auto& [key, heroPtr2] : m_segments[i][j].listHeroes)
					{
						if (heroPtr1 == heroPtr2) continue;
						heroPtr1->Eat(heroPtr2.get());
					}
				}
			}
		}*/

		//for (auto it = listBots.begin(); it != objectList.end();) {
		//	(*it)->TimeElapsed(diff);
		//	if ((*it)->m_state == States::EATEN)
		//	{
		//		/*shared_ptr<T> tempPtr = *it;
		//		weakList.push_back(tempPtr);*/
		//		it = objectList.erase(it);
		//	}
		//	else {
		//		++it;
		//	}
		//}



	}

	/*segmentInterval GameEngine::getSegmentInterval(sf::Vector2f center, sf::Vector2f sizeParamObj, sf::Vector2f segmentSize) const
	{
		Vector2f leftUpPos = { center - sizeParamObj * 0.5f };
		short leftBounder = static_cast<int>(floor(leftUpPos.x / segmentSize.x)) - 1;
		short rightBounder = static_cast<int>(floor((leftUpPos.x + sizeParamObj.x) / segmentSize.x)) + 1;

		short leftBounder = static_cast<int>(floor(static_cast<int>(leftUpPos.x) % static_cast<int>(segmentSize.x))) - 1;

		bool gapX = false;

		if (leftBounder < 0 && rightBounder >= segmentCol)
		{
			leftBounder = 0;
			rightBounder = segmentCol - 1;
			gapX = false;
		}
		else if (leftBounder < 0)
		{
			leftBounder += segmentCol;
			gapX = true;
		}
		else if (rightBounder >= segmentCol)
		{
			rightBounder -= segmentCol;
			gapX = true;
		}


		
		short topBounder = static_cast<int>(floor(leftUpPos.y / segmentSize.y)) - 1;
		short bottomBounder = static_cast<int>(floor((leftUpPos.y + sizeParamObj.y) / segmentSize.y)) + 1;


		bool gapY = false;

		if (topBounder < 0 && bottomBounder >= segmentRows)
		{
			topBounder = 0;
			bottomBounder = segmentRows - 1;
			gapY = false;
		}
		else if (topBounder < 0)
		{
			topBounder += segmentRows;
			gapY = true;
		}
		else if (bottomBounder >= segmentRows)
		{
			bottomBounder -= segmentRows;
			gapY = true;
		}

		return
		{
			leftBounder,
			rightBounder,
			topBounder,
			bottomBounder,
			gapX,
			gapY
		};
	}*/

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
	/*	else if (request.contains("action") && request["action"] == "get View Info")
		{
			string id = request["Id"].get<string>();
			auto obj = m_map->findObj(id);
			if (obj)
			{
				response["logicSize"]["x"] = m_camera.getLogicSize().x;
				response["logicSize"]["y"] = m_camera.getLogicSize().y;

				response["zoom"] = m_camera.getZoom(hero->second->getMass(), minBotAndPlayerMass);
				response["status"] = "OK";
			}
			else
			{
				response["status"] = "ERROR";
			}
		}*/

		else if (request.contains("action") && request["action"] == "get Map Info")
		{
			response["mapSize"]["x"] = MapConstants::mapWidth;
			response["mapSize"]["y"] = MapConstants::mapHeight;

			response["status"] = "OK";
		}

		//else if (request.contains("action") && request["action"] == "ask update objects")
		//{
		//	if (m_TimeUpdateListObjects == 0.0f)
		//	{
		//		m_TimeUpdateListObjects = 1000.0f;
		//		response["can update"] = true;
		//	}
		//	else
		//	{
		//		response["can updaete"] = false;
		//	}
		//	response["status"] = "OK";
		//}
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
				auto& listObjects = m_map->getNearObjects(/*player->getViewArea()*/FloatRect(0, 0, 2490, 2490));
				/*cout << listObjects.size() << endl;*/
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
		/*Bot["Splitted"] = bot.isSplitted();*/
		Bot["Speed"]["x"] = bot.getSpeed().x;
		Bot["Speed"]["y"] = bot.getSpeed().y;
		/*Bot["Mouse"]["x"] = bot.getMouse().x;
		Bot["Mouse"]["y"] = bot.getMouse().y;*/
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
	/*	Piece["maxV"] = piece.getMaxV();
		Piece["ParentId"] = piece.getParentPointer()->getID();
		Piece["isExcluded"] = piece.isExcludedFromMerge();*/
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

	//bool GameEngine::objInSegment(shared_ptr<Objects>& obj, int segFirstInd, int segSecondInd, Vector2f shift)
	//{
	//	/*Vector2f rectMinPos = m_segments[segFirstInd][segSecondInd].m_leftUpperPos;
	//	Vector2f rectMaxPos = { m_segments[segFirstInd][segSecondInd].m_leftUpperPos.x + m_segmentSize.x,  m_segments[segFirstInd][segSecondInd].m_leftUpperPos.y + m_segmentSize.y };
	//	float closestX = clamp(obj->getCenter().x + shift.x, rectMinPos.x, rectMaxPos.x);
	//	float closestY = clamp(obj->getCenter().y + shift.y, rectMinPos.y, rectMaxPos.y);

	//	float deltaX = obj->getCenter().x - closestX;
	//	float deltaY = obj->getCenter().y - closestY;

	//	float dist = deltaX * deltaX + deltaY * deltaY;

	//	return dist <= obj->getRadius() * obj->getRadius();*/
	//	return false;
	//}

	//void GameEngine::updatePlayersList(float diff)
	//{
	//	/*for (auto& i = listHeroes.begin(); i != listHeroes.end();)
	//	{
	//		(*i)->TimeElapsed(diff);
	//		if ((*i)->m_state == States::EATEN)
	//		{
	//			i++;
	//			continue;
	//		}
	//		if ((*i)->m_state == States::READY_TO_NEXT_ACTION)
	//		{
	//			i = listHeroes.erase(i);
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}*/
	//}

	//void GameEngine::updateFeedsList(float diff)
	//{
	//	/*for (auto& i = listFeeds.begin(); i != listFeeds.end();)
	//	{
	//		(*i)->TimeElapsed(diff);
	//		if ((*i)->m_state == States::EATEN)
	//		{
	//			i++;
	//			continue;
	//		}
	//		if ((*i)->m_state == States::READY_TO_NEXT_ACTION)
	//		{
	//			i = listFeeds.erase(i);
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}*/
	//}

	/*void GameEngine::allObjectsCollWithMap()
	{
		for (auto& hero : listHeroes)
		{
			if (isCollWithMap(hero.second->getCenter()))
				hero.second->setCenter(getCoorCollWithMap(hero.second->getCenter()));

			for (auto& Piece : hero.second->pieces)
			{
				if (isCollWithMap(Piece.second->getCenter()))
					Piece.second->setCenter(getCoorCollWithMap(Piece.second->getCenter()));
			}
		}

		for (auto& f : listFeeds)
		{
			if (isCollWithMap(f.second->getCenter()))
				f.second->setCenter(getCoorCollWithMap(f.second->getCenter()));
		}
		for (auto& b : listBots)
		{
			if (isCollWithMap(b.second->getCenter()))
				b.second->setCenter(getCoorCollWithMap(b.second->getCenter()));
		}
	}*/

	/*void GameEngine::calcDirMoveBot(int diff)
	{
		for (auto& i = m_map.getList<Bot>().begin(); i != m_map.getList<Bot>().end(); i++)
		{
			if (i->second->m_state == States::EATEN)
			{
				continue;
			}
			Vector2f to(0.0, 0.0);

			for (auto& j = m_map.getList<Food>().begin(); j != m_map.getList<Food>().end(); ++j)
			{
				if (j->second->m_state == States::EATEN)
					continue;
				to += calcDistBotAndObj(*i->second, *j->second, i->second->getRadius() * 0.99f);
			}

			{
				for (auto& j = m_map.getList<Thorn>().begin(); j != m_map.getList<Thorn>().end(); ++j)
				{
					to -= 4.f * calcDistBotAndObj(*i->second, *j->second, min(i->second->getRadius(), j->second->getRadius()) * 0.99f);
				}
			}
			to = getIdentityVector(to) * 100.f + i->second->getCenter();
			i->second->setPosMouse(to.x, to.y);
			i->second->TimeElapsed(diff);
		}
	}

	Vector2f GameEngine::calcDistBotAndObj(Objects & obj1, Objects & obj2, float zeroZone)
	{
		Vector2f distance = obj2.getCenter() - obj1.getCenter();
		float lenD = GetLen(distance);
		if (lenD < zeroZone)
		{
			return Vector2f(0.f, 0.f);
		}
		distance /= lenD;
		distance /= lenD;
		distance /= lenD;
		return distance;
	}

	bool GameEngine::checkAllCollision(Objects& obj)
	{
		return (checkCollision(&obj, m_map.getList<Hero>())|| checkCollision(&obj, m_map.getList<Bot>()) || checkCollision(&obj, m_map.getList<Thorn>()) || checkCollision(&obj, m_map.getList<Food>()));

	}*/
}
