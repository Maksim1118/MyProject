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

extern bool thornEeaten;
mutex m;
constexpr int fieldWidth = 1200;
constexpr int fieldHeight = 1200;
constexpr float maxMassForScale = 4000.00f;
constexpr float minMassForScale = 400.00f;
constexpr float maxScale = 1.00f;
constexpr float minScale = 2.00f;

void fillHero(const nlohmann::json& obj, Hero* & LocalPlayer);
void fillBot(const nlohmann::json& obj, Bot* & bot);
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
{
	

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
				requestMapInfo(map);
			}

			lock_guard lock(m);
			
			unordered_set<string> currentIds;

			if (response.contains("listObjects"))
			{
				for (const auto& obj : response["listObjects"])
				{
					int typeInt = obj["Type"];
					string id = obj["id"];

					currentIds.insert(id);
					ObjectsType type = static_cast<ObjectsType>(typeInt);

					if (type == ObjectsType::FOOD)
					{
						
						auto it = map.ListFoods.find(id);
						if (it == map.ListFoods.end())
						{
							Food* food = createFood(obj);
							map.ListFoods[id] = food;
							map.insertInObjectsList(food);
						}
						else
						{
							fillFood(obj, it->second);
						}
					}
					else if (type == ObjectsType::THORN) 
					{
						auto it = map.ListThorns.find(id);
						if (it == map.ListThorns.end())
						{
							Thorn* thorn = createThorn(obj);
							map.ListThorns[id] = thorn;
							map.insertInObjectsList(thorn);
						}
						else
						{
							fillThorn(obj, it->second);
						}
					}
					else if (type == ObjectsType::PIECE)
					{
						auto it = map.ListPieces.find(id);
						if (it == map.ListPieces.end())
						{
							Piece* piece = createPiece(obj);
							map.ListPieces[id] = piece;
							map.insertInObjectsList(piece);
						}
						else
						{
							fillPiece(obj, it->second);
						}
					}
					else if (type == ObjectsType::BOT)
					{
						auto it = map.ListBots.find(id);
						if (it == map.ListBots.end())
						{
							Bot* bot = createBot(obj);
							map.ListBots[id] = bot;
							map.insertInObjectsList(bot);
						}
						else
						{
							fillBot(obj, it->second);
						}
					}
					else if (type == ObjectsType::PLAYER)
					{
						auto it = map.ListPlayers.find(id);
						if (it == map.ListPlayers.end())
						{
							Hero* hero = createPlayer(obj);
							map.ListPlayers[id] = hero;
							map.insertInObjectsList(hero);
						}
						else
						{
							fillHero(obj, it->second);
							
						}
					}
				}
			}

			for (auto it = map.objects.begin(); it != map.objects.end(); )
			{		
				if (currentIds.find((*it)->getID()) == currentIds.end())
				{
					it = map.objects.erase(it);
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
	
 	fieldBGSprite.setTexture(*rec.textures[ResourceManager::Textures::FIELD]);

	thread t(requestMainInfo, ref(m_Map), ref(m_cameraInterval), ref(m_segmentsMap));
	t.detach();

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
	objects.push_back(obj);
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
	}
		
}

void GameScene::TimeElapsed(int& diff)
{
	PastTime time;

	if (m_Map.LocalPlayer != nullptr)
	{
		Vector2f dir = m_Map.LocalPlayer->GetCyclicDiff(m_Map.LocalPlayer->getCenter(), m_MousePos, 2500, 2500);
		m_Map.LocalPlayer->setDirection(dir);
	}

	if (!m_Map.ListPieces.empty())
	{
		for (auto& piece : m_Map.ListPieces)
		{
			if (m_Map.LocalPlayer)
			{
				if (piece.second->getParentID()  == m_Map.LocalPlayer->getID())
				{
					Vector2f dir = m_Map.LocalPlayer->GetCyclicDiff(piece.second->getCenter(), m_MousePos, 2500, 2500);
					piece.second->setDirection(dir);
				}
			}	
		}
	}

	timer += diff;
	if (timer > 100)
	{
		if (m_Map.LocalPlayer != nullptr)
		{
			nlohmann::json request;
			request["action"] = "ask pos mouse";
			request["Id"] = m_Map.LocalPlayer->getID();

			request["mousePosition"] = { {"x", m_MousePos.x}, {"y", m_MousePos.y}};
			sendMessage(request);
		}
		timer = 0;
	}
	time.update();
	lock_guard lock(m);
	time.update();
	m_Map.pieceToSides();
	for (const auto& o : m_Map.objects)
	{
		if(o->m_IsLive)
			o->TimeElapsed(diff);
	}

	
	if(m_Map.LocalPlayer)
		m_Map.camera.updateView(m_Map.LocalPlayer->getCenter());
	time.update();
	
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

			m_Map.LocalPlayer = hero;
			m_Map.ListPlayers[hero->getID()] = hero;
			m_Map.insertInObjectsList(hero);
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
	piece->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	piece->setRadius(obj["Radius"]);
	piece->setMass(obj["Mass"].get<float>());
	piece->setMaxV(obj["MaxV"].get<float>());
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

void fillBot(const nlohmann::json& obj, Bot* & bot)
{
	bot->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	bot->setRadius(obj["Radius"]);
	bot->setMass(obj["Mass"].get<float>());
	bot->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - bot->getCenter());
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

void fillHero(const nlohmann::json& obj, Hero* & LocalPlayer)
{
	LocalPlayer->setSplitted(obj["Splitted"].get<bool>());
	LocalPlayer->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
	LocalPlayer->setRadius(obj["Radius"].get<float>());
	LocalPlayer->setMass(obj["Mass"].get<float>());
	LocalPlayer->setMaxV(obj["MaxV"].get<float>());
	LocalPlayer->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - LocalPlayer->getCenter());
	LocalPlayer->m_IsGameOver = obj["GameOver"];

}

void GameScene::draw(RenderWindow& window)
{
	lock_guard lock(m);
	
	if (m_Map.LocalPlayer)
		m_view.setCenter(m_Map.LocalPlayer->getCenter());

	window.setView(m_view);

	for (const auto& obj : m_Map.objects)
	{
		obj->draw(window);
	}

	if(m_Map.LocalPlayer)
		m_MassText.setString("mass: " + to_string(m_Map.LocalPlayer->getRadius() * m_Map.LocalPlayer->getRadius()));
	window.draw(m_MassText);
}
