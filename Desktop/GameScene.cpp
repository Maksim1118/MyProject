#include "GameScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "Message.h"
#include "Feed.h"
#include "MyAlgorithms.hpp"
#include "PastTime.h"

#include <thread>
#include <mutex>
#include <ctime>
#include<iostream>
#include <chrono>
#include <unordered_set>

//int Options::SplitedKeyCode;
extern bool thornEeaten;
mutex m;
constexpr int fieldWidth = 1200;
constexpr int fieldHeight = 1200;
constexpr float maxMassForScale = 4000.00f;
constexpr float minMassForScale = 400.00f;
constexpr float maxScale = 1.00f;
constexpr float minScale = 2.00f;

void fillHero(const nlohmann::json& obj, Hero* & LocalPlayer, GameScene::Map & map);
void fillBot(const nlohmann::json& obj, Bot* & bot, GameScene::Map& map);
void fillPiece(const nlohmann::json& obj,Piece* & piece);
void fillFeed(const nlohmann::json& obj, Feed* & feed);
void fillFood(const nlohmann::json& obj, Food* & food);
void fillThorn(const nlohmann::json& obj, Thorn* & thorn);
Hero* createPlayer(const nlohmann::json obj);
Piece* createPiece(const nlohmann::json obj);
Feed* createFeed(const nlohmann::json obj);
Food* createFood(const nlohmann::json obj);
Thorn* createThorn(const nlohmann::json obj);
Bot* createBot(const nlohmann::json obj);


bool requestSegmentInfo(Vector2f& outSegmentSize, Vector2i& maxIndices);

Color jsonToColor(const nlohmann::json& colorJson);

void requestUpdateListObjects(unordered_set<string>& listOldId, list<Objects*>& objects, Hero*& hero);
void requestViewInfo(Camera& camera, Hero& hero);
void requestMapInfo(GameScene::Map& map);

void requestMainInfo(GameScene::Map & map, segmentInterval& segInterval, unordered_map<int, mapSegment>& segments)
{/*
	bool feedsCreated = false;
	bool foodsCreated = false;
	bool thornCreated = false;
	list<shared_ptr<Piece>> tempHeroPieces;
	list<shared_ptr<Piece>> tempBotPieces;
	vector<shared_ptr<Objects>> tempObjects; 

	bool firstTime = true;*/
	

	while (true)
	{
		if (map.LocalPlayer == nullptr)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			continue;
		}
		nlohmann::json listObjects;
		listObjects["action"] = "get object";
		listObjects["Id"] = map.LocalPlayer->getID();
		nlohmann::json response = message.process(listObjects);
		if (response.is_null())
		{
			continue;
		}
		else
		{
			
			{
				lock_guard lock(m);
				requestViewInfo(map.camera, *map.LocalPlayer);
			}
			
			{
				lock_guard lock(m);
				requestMapInfo(map);
			}

			lock_guard lock(m);
			/*if (response.contains("ListFoods"))
			{
				for (const auto& f : response["ListFoods"])
				{
					for (const auto& vecInd : f["SegIndices"])
					{
						int firstInd = vecInd["x"].get<int>();
						int secondInd = vecInd["y"].get<int>();

						if (segInterval.isInterval(firstInd, secondInd))
						{
							int totalIndex = firstInd * secondInd + secondInd;
							auto& segment = segments[totalIndex];
							string foodId = f["id"].get<string>();
							newFoodIds.insert(foodId);

							auto itFood = segment.foods.find(foodId);
							if (itFood == segment.foods.end())
							{
								Food* food = createFood(f);
								segment.foods[food->getID()] = food;
								segment.objects.push_back(food);
							}
							else
							{
								fillFood(f, itFood->second);
							}
						}
					}
				}

				for (int i = segInterval.left; i < segInterval.right; ++i)
				{
					for (int j = segInterval.top; j < segInterval.bottom; ++j)
					{
						int totalIndex = i * j + j;
						auto segment = segments.find(totalIndex);
						if (segment != segments.end())
						{
							vector<string>& listOldIds = segment->second.listOldIds;
							auto& foods = segment->second.foods;
							auto& objects = segment->second.objects;

							for (auto it = objects.begin(); it != objects.end(); )
							{
								const std::string& id = (*it)->getID();

								if (newFoodIds.find(id) == newFoodIds.end())
								{
									delete it->second;
									objects.erase(id);
									it = foods.erase(it);
								}
								else
								{
									++it;
								}
							}
						}
					}
				}
			}


			for (int i = segInterval.left; i < segInterval.right; ++i)
			{
				for (int j = segInterval.top; j < segInterval.bottom; ++j)
				{
					int totalIndex = i * j + j;
					auto segment = segments.find(totalIndex);
					if (segment != segments.end())
					{

					}
				}
			}*/

			if (response.contains("ListPieces"))
			{
				for (auto& [id, piecePtr] : map.ListPieces)
				{
					piecePtr->m_isMarked = true;
				}

				for (const auto& p : response["ListPieces"])
				{
					string pieceId = p["id"].get<string>();
					auto& insert_piece = map.ListPieces.insert(make_pair(pieceId, nullptr));
					if (insert_piece.second)
					{
						Piece* piece = createPiece(p);
						piece->m_isMarked = false;
						insert_piece.first->second = piece;
						map.insertInObjectsList(piece);
					}
					else
					{
						fillPiece(p, insert_piece.first->second);
						insert_piece.first->second->m_isMarked = false;
					}
				}
			}
			for (auto it = map.ListPieces.begin(); it != map.ListPieces.end();)
			{
				if (it->second->m_isMarked)
				{
					map.ListOldId.insert(it->first);
					it = map.ListPieces.erase(it);
				}
				else
				{
					++it;
				}
			}

			if (response.contains("ListHeroes"))
			{
				for (auto& [id, playerPtr] : map.ListPlayers)
				{
					playerPtr->m_isMarked = true;
				}

				for (const auto& jsonHero: response["ListHeroes"])
				{
					string playerId = jsonHero["id"].get<string>();
					auto& insert_res = map.ListPlayers.insert(make_pair(playerId, nullptr));
					if (insert_res.second)
					{
						Hero* player = createPlayer(jsonHero);
						player->m_isMarked = false;
						insert_res.first->second = player;
						map.insertInObjectsList(player);
					}
					else
					{	
						fillHero(jsonHero, insert_res.first->second, map);
						insert_res.first->second->m_isMarked = false;
						/*if (insert_res.first->second->isSplitted())
						{
							insert_res.first->second->m_isMarked = true;
						}
						else
						{
							insert_res.first->second->m_isMarked = false;
						}*/
					}
					
				}

				for (auto it = map.ListPlayers.begin(); it != map.ListPlayers.end();)
				{
					if (it->second->m_isMarked)
					{
						map.ListOldId.insert(it->first);
						it = map.ListPlayers.erase(it);
					}
					else
					{
						++it;
					}
				}
				
			}
				
			if (response.contains("ListFoods"))
			{
				for (auto& [id, foodPtr] : map.ListFoods)
				{
					foodPtr->m_isMarked = true;
				}

				for (const auto& f : response["ListFoods"])
				{
					string FoodId = f["id"].get<string>();
					auto& insert_res = map.ListFoods.insert(make_pair(FoodId, nullptr));
					if (insert_res.second)
					{
						Food* food = createFood(f);
						food->m_isMarked = false;
						insert_res.first->second = food;
						map.insertInObjectsList(food);
					}
					else
					{
						fillFood(f, insert_res.first->second);
						insert_res.first->second->m_isMarked = false;
					}
				}

				for (auto it = map.ListFoods.begin(); it != map.ListFoods.end();)
				{
					if (it->second->m_isMarked)
					{
						map.ListOldId.insert(it->first);
						it = map.ListFoods.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			if (response.contains("ListBots"))
			{
				for (auto& [id, playerPtr] : map.ListBots)
				{
					playerPtr->m_isMarked = true;
				}

				for (const auto& jsonBot : response["ListBots"])
				{
					string botId = jsonBot["id"].get<string>();
					auto& insert_res = map.ListBots.insert(make_pair(botId, nullptr));
					if (insert_res.second)
					{
						Bot* bot = createPlayer(jsonBot);
						bot->m_isMarked = false;
						insert_res.first->second = bot;
						map.insertInObjectsList(bot);
					}
					else
					{
						fillBot(jsonBot, insert_res.first->second, map);
						insert_res.first->second->m_isMarked = false;
						/*if (insert_res.first->second->isSplitted())
						{
							insert_res.first->second->m_isMarked = true;
						}
						else
						{
							insert_res.first->second->m_isMarked = false;
						}*/
					}

				}

				for (auto it = map.ListBots.begin(); it != map.ListBots.end();)
				{
					if (it->second->m_isMarked)
					{
						map.ListOldId.insert(it->first);
						it = map.ListBots.erase(it);
					}
					else
					{
						++it;
					}
				}

			}

			if (response.contains("ListThorns"))
			{
				for (auto& [id, thornPtr] : map.ListThorns)
				{
					thornPtr->m_isMarked = true;
				}

				for (const auto& t : response["ListThorns"])
				{
					string thornId = t["id"].get<string>();
					auto& insert_res = map.ListThorns.insert(make_pair(thornId, nullptr));
					if (insert_res.second)
					{
						Thorn* thorn = createThorn(t);
						thorn->m_isMarked = false;
						insert_res.first->second = thorn;
						map.insertInObjectsList(thorn);
					}
					else
					{
						fillThorn(t, insert_res.first->second);
						insert_res.first->second->m_isMarked = false;
					}
				}

				for (auto it = map.ListThorns.begin(); it != map.ListThorns.end();)
				{
					if (it->second->m_isMarked)
					{
						map.ListOldId.insert(it->first);
						it = map.ListThorns.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

	

			for (auto it = map.objects.begin(); it != map.objects.end();)
			{
				string objID = (*it)->getID();
				if (find(map.ListOldId.begin(), map.ListOldId.end(),
					(*it)->getID()) != map.ListOldId.end())
				{
					if (map.LocalPlayer != nullptr && (*it)->getID() != map.LocalPlayer->getID())
					{
						delete* it;
						it = map.objects.erase(it);
						map.ListOldId.erase(objID);
					}
				}
				else
				{
					++it;
				}
			}

			timSort<Objects>(map.objects, [](const Objects* a, const Objects* b)
				{
					return a->getRadius() < b->getRadius();
				});
			/*if (response.contains("ListFeeds"))
			{
				unordered_set<string> newFeeds;
			
				for (const auto& f : response["ListFeeds"])
				{ 
					string FeedId = f["id"];
					newFeeds.insert(FeedId);
					auto itFeed = find_if(map.ListFeeds.begin(), map.ListFeeds.end(), [FeedId](const Feed* o)
						{
							return o->getID() == FeedId;
						});
					if (itFeed == map.ListFeeds.end())
					{
						Feed* feed = createFeed(f);
						map.ListFeeds.push_back(feed);
						map.insertInObjectsList(feed);
					}
					else
					{
						fillFeed(f, *itFeed);
					}
				}

				unordered_set<string>& listOldId = map.ListOldId;

				map.ListFeeds.erase(remove_if(map.ListFeeds.begin(), map.ListFeeds.end(), [&newFeeds,&listOldId](Feed* & feed)
					{
						bool toRemove = newFeeds.find(feed->getID()) == newFeeds.end();
						if (toRemove)
						{
							feed->m_IsLive = false;
							listOldId.insert(feed->getID());
						}
						return toRemove;
					}), map.ListFeeds.end());
			}

			if (response.contains("ListThorns"))
			{	
				unordered_set<string> newThorns;
				for (const auto& t : response["ListThorns"])
				{
					string ThornId = t["id"];
					newThorns.insert(ThornId);
					auto itThorn = find_if(map.ListThorns.begin(), map.ListThorns.end(), [ThornId](const Thorn* o)
						{
							return o->getID() == ThornId;
						});
					if (itThorn == map.ListThorns.end())
					{
						Thorn* thorn = createThorn(t);
						map.ListThorns.push_back(thorn);
						map.insertInObjectsList(thorn);
					}
					else
					{
						fillThorn(t, *itThorn);
					}
				}

				unordered_set<string>& listOldId = map.ListOldId;

				map.ListThorns.erase(remove_if(map.ListThorns.begin(), map.ListThorns.end(), [&newThorns, &listOldId](Thorn*& thorn)
					{
						bool toRemove = newThorns.find(thorn->getID()) == newThorns.end();
						if (toRemove)
						{
							thorn->m_IsLive = false;
							listOldId.insert(thorn->getID());
						}
						return toRemove;
					}), map.ListThorns.end());
			}
			if (response.contains("ListBots"))
			{
				unordered_set<string> newBots;
				for (const auto& obj : response["ListBots"])
				{
					string botId = obj["id"];
					newBots.insert(botId);
					auto itBot = find_if(map.ListBots.begin(), map.ListBots.end(), [botId](const Bot* o)
						{ 
							return o->getID() == botId;
						});
					if (itBot == map.ListBots.end())
					{
						Bot* bot = createBot(obj);
						map.ListBots.push_back(bot);
						map.insertInObjectsList(bot);
					}
					else
					{
						fillBot(obj, *itBot, map);
					}
				}
				unordered_set<string>& listOldId = map.ListOldId;

				map.ListBots.erase(remove_if(map.ListBots.begin(), map.ListBots.end(), [&newBots, &listOldId](Bot*& bot)
					{
						bool toRemove = newBots.find(bot->getID()) == newBots.end();
						if (toRemove)
						{
							bot->m_IsLive = false;
							listOldId.insert(bot->getID());
						}
						return toRemove;
					}), map.ListBots.end());
			};*/

			
		}

		this_thread::sleep_for(chrono::milliseconds(300));
	}
	
}

bool requestSegmentInfo(Vector2f& outSegmentSize, Vector2i& maxIndices)
{
	nlohmann::json requestSegmentSize;
	requestSegmentSize["action"] = "get Segment Info";
	nlohmann::json response = message.process(requestSegmentSize);
	if (!response.is_null())
	{
		if (response.contains("segmentSize"))
		{
			auto segSize = response["segmentSize"];
			outSegmentSize.x = segSize["x"].get<float>();
			outSegmentSize.y = segSize["y"].get<float>();
		}
		if (response.contains("maxIndices"))
		{
			auto maxSegIndices = response["maxIndices"];
			maxIndices.x = maxSegIndices["x"].get<int>();
			maxIndices.y = maxSegIndices["y"].get<int>();
		}
		return true;
	}
	return false;
}

void requestViewInfo(Camera& camera, Hero& hero)
{
	nlohmann::json request;
	request["action"] = "get View Info";
	request["Id"] = hero.getID();
	nlohmann::json response = message.process(request);
	if (!response.is_null())
	{
		Vector2i logicSize{};
		float zoom{ 0 };
		if (response.contains("logicSize"))
		{
			logicSize = {response["logicSize"]["x"].get<int>(), response["logicSize"]["y"].get<int>()};
		}
		if (response.contains("zoom"))
		{
			zoom = response["zoom"].get<float>();
			camera.setTargetZoom(zoom);
		}
	}
}

void requestMapInfo(GameScene::Map& map)
{
	nlohmann::json request;
	request["action"] = "get Map Info";
	nlohmann::json response = message.process(request);
	if (!response.is_null())
	{
		if (response.contains("mapSize"))
		{
			map.size = { response["mapSize"]["x"].get<float>(), response["mapSize"]["y"].get<float>() };
		}
	}
}

GameScene::GameScene(SceneManager* manager): _manager(manager), m_segmentSize{0.f, 0.f}
{

	text.setString(L"Вы Проиграли");
	text.setFont(*rec.font);
	text.setCharacterSize(80);
	text.setFillColor(Color::Red);

	m_MassText.setFont(*rec.font);
	m_MassText.setCharacterSize(25);
	m_MassText.setPosition(200.f, 200.f);
	m_MassText.setFillColor(Color::Black);
	
	/*if (!requestSegmentInfo(m_segmentSize, m_maxSegIndices))
	{
		throw::runtime_error("fail request segment info");
	}*/

	/*settingCameraInterval();*/
	
 	fieldBGSprite.setTexture(*rec.textures[ResourceManager::Textures::FIELD]);

	thread t(requestMainInfo, ref(m_Map), ref(m_cameraInterval), ref(m_segmentsMap));
	t.detach();


	/*thread t2(requestUpdateListObjects, ref(m_Map.ListOldId), ref(m_Map.objects), ref(m_Map.LocalPlayer));
	t2.detach();*/

}

void GameScene::Map::pieceToSides()
{
	for (auto& piece : ListPieces)
	{
		Vector2f center = piece.second->getCenter();
		const auto& parentId1 = piece.second->getParentID();

		for (auto& piece2 : ListPieces)
		{
			if (piece.first == piece2.first)
			{
				continue;
			}
			const auto& parentId2 = piece2.second->getParentID();

			if (parentId1 != parentId2)
			{
				continue;
			}

			if (piece.second->isExclude() || piece2.second->isExclude())
			{
				float lenPiece2 = MoveObject::GetLen(piece.second->getCenter(), piece2.second->getCenter(), { fieldWidth, fieldHeight });
				float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
				if (lenPiece2 < sumRadius)
				{
					Vector2f toPiece21 = MoveObject::getIdentityVector(piece2.second->getCenter() - center);
					piece2.second->setCenter(piece2.second->getCenter() + toPiece21 * (sumRadius - lenPiece2) / (float)2);
					piece.second->setCenter(piece.second->getCenter() - toPiece21 * (sumRadius - lenPiece2) / (float)2);
				}
			}

		}
	}
}


Vector2f GameScene::Map::calcAttractionForcePiece(Piece* &piece, const Vector2f& mousePos)
{
	Vector2f F(0, 0);
	Vector2f center = piece->getCenter();
	Vector2f dir = mousePos - center;
	F += MoveObject::identityVector(dir);

	const auto& parentId = piece->getParentID();

	for (auto& piece2 : ListPieces) 
	{
		
		if (piece == piece2.second) 
		{
			continue;
		}

		const auto& parentId2 = piece2.second->getParentID();
		if (parentId != parentId2)
		{
			continue;
		}

		float lenPiece2 = MoveObject::GetLen(piece->getCenter(), piece2.second->getCenter(), size);
		float sumRadius = piece->getRadius() + piece2.second->getRadius();
		float minDist = sumRadius * 2;
		/*if (lenPiece2 > sumRadius)
		{
			Vector2f toPiece21 = getIdentityVector(piece2.second->getCenter() - center);
			F += toPiece21 * (float)0.01;
		}*/
		if (lenPiece2 > minDist)
		{
			Vector2f toPiece21 = MoveObject::identityVector(piece2.second->getCenter() - center);
			float excess = lenPiece2 - minDist;
			float strength = pow(excess, 3) * 0.0005f;

			F += toPiece21 * strength;
		}
	}
	return F;
}


Vector2f GameScene::Map::calcAccelerationPiece(const Vector2f& F, Piece* & piece)
{
	float mass = piece->getMass();
	if (mass == 0.f)
	{
		return { 0.f, 0.f };
	}

	Vector2f a = F / mass;
	float lenA = MoveObject::GetLen(a);
	float maxA = 2.0 / mass;
	if (lenA > maxA) {
		a = a / lenA * (float)maxA;
	}
	return a;
}

Vector2f GameScene::Map::calcSpeedPiece(const Vector2f& a, const Vector2f& mousePos, int diff,  Piece* & piece)
{
	Vector2f v{ 0.f,0.f };
	v = piece->getSpeed() + a * (float)diff * 1.f;
	float lenV = MoveObject::GetLen(v);
	float maxV = piece->getMaxV();
	/*float lenMouse = GetDist(_Mouse,getCenter());*/
	float lenMouse = MoveObject::GetLen(mousePos, piece->getCenter(), size);
	if (lenMouse < 40.f)
	{
		maxV *= (lenMouse / 40.f);
	}
	if (lenV > maxV) {
		/*v *= (float)maxV / lenV;*/
		v = v / lenV * maxV;
	}
	return v;
}


void GameScene::Map::fillListObjects()
{

	for (const auto& player : ListPlayers)
	{
		objects.emplace_back(player.second);
	}

	for (const auto& bot : ListBots)
	{
		objects.emplace_back(bot.second);
	}

	for (const auto& feed : ListFeeds)
	{
		objects.emplace_back(feed.second);
	}

	for (const auto& food : ListFoods)
	{
		objects.emplace_back(food.second);
	}

	for (const auto& thorn : ListThorns)
	{
		objects.emplace_back(thorn.second);
	}
}

void GameScene::Map::insertInObjectsList(Objects* obj)
{
	auto pos = findPos<Objects>(objects, obj, [](const Objects* a, const Objects* b)
		{
			return a->getRadius() < b->getRadius();
		});
	objects.insert(pos, obj);
}

void getRequest(const nlohmann::json& request)
{
	nlohmann::json response = message.process(request);
}

void sendMessage(const nlohmann::json& request)
{
	thread t(&getRequest, request);
	t.detach();
}

void GameScene::HandleEvent(Event& event)
{

	nlohmann::json request;
	if (event.type == sf::Event::KeyPressed)
	{
		
		if (event.key.scancode == sf::Keyboard::Scan::Escape)
		{
			_manager->SetMainScene();
		}
		// TO DO

		if (event.key.code == Keyboard::Space)
		{
			request["action"] = "split";
			request["Id"] = m_Map.LocalPlayer->getID();
			sendMessage(request);
		}
		if (event.key.code == Keyboard::E)
		{
			request["action"] = "feed";
			request["Id"] = m_Map.LocalPlayer->getID();
			sendMessage(request);
		}
		/*if (event.key.code == Options::SplitedKeyCode)	
		{
			request["action"] = "split";
			request["Id"] = m_Map.LocalPlayer->getID();
			nlohmann::json response = message.process(request);

			
		}
		if (event.key.code == Options::FeededKeyCode)
		{		
			request["action"] = "feed";
			request["Id"] = m_Map.LocalPlayer->getID();
			nlohmann::json response = message.process(request);
		}*/
	}
	if (event.type == sf::Event::MouseMoved)
	{
		if(m_Map.LocalPlayer != nullptr)
			m_Map.LocalPlayer->setPosMouse(m_MousePos.x, m_MousePos.y);
	}
}

void GameScene::TimeElapsed(int& diff)
{
	PastTime time;
	
	

	timer += diff;
	if (timer > 100)
	{
		if (m_Map.LocalPlayer != nullptr)
		{
			nlohmann::json request;
			request["action"] = "ask pos mouse";
			request["Id"] = m_Map.LocalPlayer->getID();
			request["mousePosition"] = { {"x",(int)m_MousePos.x}, {"y", (int)m_MousePos.y} };
			sendMessage(request);
		}
		timer = 0;
	}
	time.update();
	cout << "mouse: " << time.getDeltaTime() << endl;
	lock_guard lock(m);
	time.update();
	cout << "mutexLock: " << time.getDeltaTime() << endl;
	/*for (auto& piece : m_Map.ListPieces)
	{
		Vector2f F = m_Map.calcAttractionForcePiece(piece.second, posMouse);
		Vector2f a = m_Map.calcAccelerationPiece(F, piece.second);
		Vector2f v = m_Map.calcSpeedPiece(a, posMouse, diff, piece.second);
		piece.second->setSpeed(v);
	}*/
	m_Map.pieceToSides();
	m_Map.camera.updateZoom(diff);
	for (const auto& o : m_Map.objects)
	{
		if(o->m_IsLive)
			o->TimeElapsed(diff);
	}
	if(m_Map.LocalPlayer)
		m_Map.camera.updateView(m_Map.LocalPlayer->getCenter());
	time.update();
	cout << "objectsTimeElapsed: " << time.getDeltaTime() << endl;

	/*cameraUpdate(diff);*/
	
}

Vector2f GameScene::getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height)
{

	if (pos.y < 0.f) {
		pos.y = 0.f;
	}
	else if (pos.y + Height > fieldHeight) {
		pos.y = fieldHeight - Height;
	}
	if (pos.x < 0.f) {
		pos.x = 0.f;
	}
	else if (pos.x + Width > fieldWidth) {
		pos.x = fieldWidth - Width, pos.y;
	}
	return pos;
}

void GameScene::setActive(const string& playerName)
{
	nlohmann::json request;
	request["action"] = "create Player";
	request["Name"] = playerName;
	nlohmann::json response = message.process(request);
	if (response["status"] == "OK")
	{
		if (response.contains("Player"))
		{
			Hero* hero = createPlayer(response["Player"]);
			/*m_Map.ListPlayers.emplace_back(hero);
			m_Map.insertInObjectsList(hero);*/

			m_Map.LocalPlayer = hero;
			m_Map.ListPlayers[hero->getID()] = hero;
			m_Map.insertInObjectsList(hero);
			
			/*list<shared_ptr<Piece>> tempHeroPieces;
			vector<shared_ptr<Objects>> tempObjects;
			fillHero(response["Hero"], tempObjects, tempHeroPieces, m_Map.objects, m_Map.LocalPlayer);*/
		}
	}
	
}

std::pair<int, int> GameScene::getSymetricRange(int count) const
{
	int half = count / 2;
	if (count % 2 == 0)
	{
		return { -half, half - 1 };
	}
	return { -half, half };
}

//void GameScene::cameraUpdate(float diff)
//{
//	m_LocalCamera.setCenter(m_Map.LocalPlayer->getCenter());
//	m_LocalCamera.updateZoom(diff, m_Map.LocalPlayer->getRadius(), 20.f);
//}
//
//void GameScene::settingCameraInterval()
//{
//	Vector2f leftUpPos = m_LocalCamera.getCenter()- m_LocalCamera.getSize() * 0.5f;
//	m_cameraInterval.left = max(static_cast<int>(floor(leftUpPos.x / m_segmentSize.x)) - 1, 0);
//	m_cameraInterval.right = min(static_cast<int>(floor(leftUpPos.x + m_LocalCamera.getSize().x / m_segmentSize.x)) + 1, m_maxSegIndices.x);
//
//	m_cameraInterval.top = max(static_cast<int>(floor(leftUpPos.y / m_segmentSize.y)) - 1, 0);
//	m_cameraInterval.bottom = min(static_cast<int>(floor(leftUpPos.y + m_LocalCamera.getSize().y / m_segmentSize.y)) + 1, m_maxSegIndices.y);
//}

Hero* createPlayer(nlohmann::json obj)
{

	Hero* hero = new Hero(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["Color"], obj["Name"], obj["id"]);
	hero->setMass(obj["Mass"].get<float>());
	return hero;
}

Piece* createPiece(nlohmann::json obj)
{
	Piece* piece = new Piece(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"].get<float>(), obj["Color"].get<int>(), obj["id"].get<string>(), obj["ParentId"].get<string>());
	piece->setMass(obj["Mass"].get<float>());
	piece->setExclude(obj["isExcluded"].get<bool>());
	return piece;
}

Feed* createFeed(nlohmann::json obj)
{
	Feed* feed = new Feed(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"],obj["id"]);
	feed->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	feed->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - feed->getCenter());
	return feed;
}

Food* createFood(nlohmann::json obj)
{
	Food* food = new Food(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["id"]);
	food->setColor(jsonToColor(obj["Color"]));
	return food;
}

Thorn* createThorn(nlohmann::json obj)
{
	Thorn* thorn = new Thorn(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"],obj["id"]);
	return thorn;
}

Bot* createBot(nlohmann::json obj)
{
	Bot* bot = new Bot(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["Color"], obj["Name"], obj["id"]);
	bot->setMass(obj["Mass"].get<float>());
	return bot;
}

void fillPiece(const nlohmann::json& obj, Piece* & piece)
{
	piece->setMaxV(obj["maxV"]);
	piece->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	piece->setRadius(obj["Radius"]);
	piece->setMass(obj["Mass"].get<float>());
	piece->setExclude(obj["isExcluded"].get<bool>());
	piece->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - piece->getCenter());
}

void fillFeed(const nlohmann::json& obj, Feed* & feed)
{
	if (feed->getDiff().x == 0.f, feed->getDiff().y == 0.f)
	{
		return;
	}
	feed->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	feed->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - feed->getCenter());
}

void fillBot(const nlohmann::json& obj, Bot* & bot, GameScene::Map& map)
{
	bot->setSplitted(obj["Splitted"].get<bool>());
	bot->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	bot->setRadius(obj["Radius"]);
	bot->setMass(obj["Mass"].get<float>());
	bot->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - bot->getCenter());

	//unordered_set<string> newPieces;
	//if (obj["Splitted"] == true)
	//{

	//	for (auto& p : obj["Pieces"])
	//	{
	//		string pieceId = p["id"];
	//		newPieces.insert(pieceId);
	//		auto itPiece = find_if(bot->pieces.begin(), bot->pieces.end(), [pieceId](const Piece* piece)
	//			{
	//				return piece->getID() == pieceId;
	//			});
	//		if (itPiece == bot->pieces.end())
	//		{
	//			Piece* piece = createPiece(p);
	//			bot->pieces.push_back(piece);
	//			map.insertInObjectsList(piece);
	//		}
	//		else
	//		{
	//			fillPiece(p, *itPiece);
	//		}
	//	}
	//}
	//bot->pieces.erase(remove_if(bot->pieces.begin(), bot->pieces.end(), [&newPieces, &map](Piece*& piece)
	//	{
	//		bool toRemove = newPieces.find(piece->getID()) == newPieces.end();
	//		if (toRemove)
	//		{
	//			piece->m_IsLive = false;
	//			map.ListOldId.insert(piece->getID());
	//		}
	//		return toRemove;
	//	}), bot->pieces.end());
}

Color jsonToColor(const nlohmann::json& colorJson)
{
	return Color(
		colorJson.at("r").get<Uint8>(),
		colorJson.at("g").get<Uint8>(),
		colorJson.at("b").get<Uint8>(),
		colorJson.at("a").get<Uint8>());
}

void requestUpdateListObjects(unordered_set<string>& listOldId, list<Objects*>& objects, Hero*& hero)
{
	while (true)
	{
		nlohmann::json request;
		request["action"] = "ask update objects";
		nlohmann::json response = message.process(request);
		if (response["status"] == "OK")
		{
			if (response.contains("can update") && response["can update"] == true)
			{
				lock_guard<mutex> lock(m);
				for (auto it = objects.begin(); it != objects.end();)
				{
					string objID = (*it)->getID();
					if (find(listOldId.begin(), listOldId.end(),
						(*it)->getID()) != listOldId.end())
					{
						if (hero != nullptr && (*it)->getID() != hero->getID())
						{
							delete* it;
							it = objects.erase(it);
							listOldId.erase(objID);
						}
					}
					else
					{
						++it;
					}
				}

				timSort<Objects>(objects, [](const Objects* a, const Objects* b)
					{
						return a->getRadius() < b->getRadius();
					});
				/*list<shared_ptr<Piece>> tempHeroPieces;
				vector<shared_ptr<Objects>> tempObjects;
				fillHero(response["Hero"], tempObjects, tempHeroPieces, m_Map.objects, m_Map.LocalPlayer);*/
			}
		}
	}
}

void fillFood(const nlohmann::json& obj, Food* & food)
{
	food->setCenter(Vector2f(obj["Center"]["x"], obj["Center"]["y"]));
}

void fillThorn(const nlohmann::json& obj, Thorn* & thorn)
{
	thorn->setCenter(Vector2f(obj["Center"]["x"].get<float>(), obj["Center"]["y"].get<float>()));
}

void fillHero(const nlohmann::json& obj, Hero* & LocalPlayer, GameScene::Map& map)
{
	/*auto itHero = find_if(m.begin(), objects.end(), [heroId](const shared_ptr<Objects>& o)
		{
			return o->getID() == heroId;
		});*/

	LocalPlayer->setSplitted(obj["Splitted"].get<bool>());
	LocalPlayer->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	LocalPlayer->setRadius(obj["Radius"].get<float>());
	LocalPlayer->setMass(obj["Mass"].get<float>());
	/*LocalPlayer->setCurrentMass(static_cast<float>(obj["Mass"]));*/
	LocalPlayer->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - LocalPlayer->getCenter());
	LocalPlayer->m_IsGameOver = obj["GameOver"];

	

	/*shared_ptr<Hero> hero;
	if (itHero == objects.end())
	{
		hero = make_shared<Hero>(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["Color"], obj["Name"], obj["id"]);
		LocalPlayer = hero;
		hero->isSplitted(obj["Splitted"]);
		float SpeedX = obj["Speed"]["x"];
		float SpeedY = obj["Speed"]["y"];
		hero->setCurrentMass(static_cast<float> (obj["Mass"]));
		hero->setSpeed(Vector2f(SpeedX, SpeedY));
		hero->m_IsGameOver = obj["GameOver"];
		tempObjects.push_back(hero);
	}
	else
	{
		hero = dynamic_pointer_cast<Hero>(*itHero);
		if (hero)
		{
			hero->isSplitted(obj["Splitted"]);
			hero->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
			hero->setRadius(obj["Radius"]);
			hero->setCurrentMass(static_cast<float>(obj["Mass"]));
			hero->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - itHero->get()->getCenter());
			hero->m_IsGameOver = obj["GameOver"];
			tempObjects.push_back(hero);
		}
	}*/
}

void GameScene::draw(RenderWindow& window)
{	
	lock_guard lock(m);
	/*m_LocalCamera.updateSize((Vector2f)window.getSize());*/
	window.setView(m_Map.camera.getView());

	{
		int tilesX = static_cast<int>(ceil(m_Map.camera.getView().getSize().x / fieldBGSprite.getLocalBounds().width)) + 2;
		int tilesY = static_cast<int>(ceil(m_Map.camera.getView().getSize().y / fieldBGSprite.getLocalBounds().height)) + 2;

		int startX = static_cast<int>(floor((m_Map.camera.getView().getCenter().x - m_Map.camera.getView().getSize().x / 2) / fieldBGSprite.getLocalBounds().width));
		int startY = static_cast<int>(floor((m_Map.camera.getView().getCenter().y - m_Map.camera.getView().getSize().y / 2) / fieldBGSprite.getLocalBounds().height));

		for (int i = startX; i < startX + tilesX; ++i)
		{
			for (int j = startY; j < startY + tilesY; ++j)
			{
				fieldBGSprite.setPosition(fieldBGSprite.getLocalBounds().width * i, fieldBGSprite.getLocalBounds().height * j);
				window.draw(fieldBGSprite);
			}
		}
	}

	for (const auto& obj : m_Map.objects)
	{
		if (obj)
		{
			if (obj == m_Map.LocalPlayer)
			{
				obj->shiftPos();
				obj->draw(window);
			}
			else
			{

				for (int i = -1; i <= 1; ++i)
				{
					for (int j = -1; j <= 1; ++j)
					{
						obj->shiftPos(m_Map.size.x * i, m_Map.size.y * j);
						obj->draw(window);
					}
				}
			}
		}
	
	}
	

	if(m_Map.LocalPlayer)
		m_MassText.setString("mass: " + to_string(m_Map.LocalPlayer->getRadius() * m_Map.LocalPlayer->getRadius()));
	window.draw(m_MassText);

	/*if (m_Map.LocalPlayer != nullptr && m_Map.LocalPlayer->m_IsGameOver)
	{
		text.setPosition(Options::_WindowWidth / 2 - text.getGlobalBounds().width / 2, Options::_WindowHeight / 2 - text.getGlobalBounds().height / 2);
		window.draw(text);
	}*/
	/*if(_GameOver)
	{
		text.setPosition(Options::_WindowWidth / 2 - text.getGlobalBounds().width / 2, Options::_WindowHeight/2 - text.getGlobalBounds().height / 2);
		window.draw(text);
	}*/
}
