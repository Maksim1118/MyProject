#include "GameScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "Message.h"
#include "Feed.h"

#include <thread>
#include <mutex>
#include <ctime>
#include<iostream>
#include <chrono>

int Options::SplitedKeyCode;
extern bool thornEeaten;
mutex m;
constexpr int fieldWidth = 1200;
constexpr int fieldHeight = 1200;
constexpr float maxMassForScale = 4000.00f;
constexpr float minMassForScale = 400.00f;
constexpr float maxScale = 1.00f;
constexpr float minScale = 2.00f;


void requestObjects(vector<shared_ptr<Objects>>& objects, Text& HeroMass, bool& GameOver, float & scale, Vector2f & CameraPos, shared_ptr<Hero>& _hero)
{
	bool feedsCreated = false;
	bool foodsCreated = false;
	bool thornCreated = false;
	list<shared_ptr<Piece>> tempHeroPieces;
	list<shared_ptr<Piece>> tempBotPieces;
	vector<shared_ptr<Objects>> tempObjects;
	while (true)
	{
		nlohmann::json listObjects;
		listObjects["action"] = "get object";
		nlohmann::json response = message.process(listObjects);
		if (response.is_null())
		{
			continue;
		}
		else
		{
			lock_guard lock(m);
			if (response.contains("Hero"))
			{
				
				auto obj = response["Hero"];
				GameOver = obj["GameOver"];
				int heroId = static_cast<int>(obj["id"]);
				auto itHero = find_if(objects.begin(), objects.end(), [heroId](const shared_ptr<Objects>& o)
					{
						return o->getID() == heroId;
					});
				shared_ptr<Hero> hero;
				int massHero = static_cast<int>(obj["Mass"]);
				HeroMass.setString("mass Hero: " + to_string(massHero));
				if (itHero == objects.end())
				{
					hero = make_shared<Hero>(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["Color"], obj["Name"], obj["id"]);
					_hero = hero;
					hero->isSplitted(obj["Splitted"]);
					float SpeedX = obj["Speed"]["x"];
					float SpeedY = obj["Speed"]["y"];
					hero->setSpeed(Vector2f(SpeedX, SpeedY));
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
						hero->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - itHero->get()->getCenter());
						tempObjects.push_back(hero);
					}
				}
				if (obj["Splitted"] == true)
				{
					
					for (auto& p : obj["Pieces"])
					{
						int pieceId = static_cast<int>(p["id"]);
						auto itPiece = find_if(objects.begin(), objects.end(), [pieceId](const shared_ptr<Objects>& o)
							{
								return o->getID() == pieceId;
							});
						shared_ptr<Piece> piece;
						if (itPiece == objects.end())
						{
							piece = make_shared<Piece>(Vector2f(p["Center"]["x"], p["Center"]["y"]), p["Radius"], p["Color"], p["id"]);
							piece->setSpeed(Vector2f(p["Speed"]["x"], p["Speed"]["y"]));
							piece->setMaxV(p["maxV"]);
							tempHeroPieces.push_back(piece);
							tempObjects.push_back(piece);
						}
						else
						{
							piece = dynamic_pointer_cast<Piece>(*itPiece);
							if (piece)
							{
								piece->setMaxV(p["maxV"]);
								piece->setSpeed(Vector2f(p["Speed"]["x"], p["Speed"]["y"]));
								piece->setRadius(p["Radius"]);
								auto it = find_if(hero->pieces.begin(), hero->pieces.end(), [piece](const shared_ptr<Piece>& pTemp)
									{
										return pTemp->getID() == piece->getID();
									});
								if (it != hero->pieces.end())
								{
									piece->setDifference(Vector2f(p["Center"]["x"], p["Center"]["y"]) - it->get()->getCenter());
								}
								tempObjects.push_back(piece);
								tempHeroPieces.push_back(piece);
							}
							
						}
					}
					hero->pieces = tempHeroPieces;
					tempHeroPieces.clear();
				}
				/*for (const auto& f : obj["Feeds"])
				{
					if (f["state"] != 0)
						continue;
					int FeedId = static_cast<int>(f["id"]);
					auto itFeed = find_if(objects.begin(), objects.end(), [FeedId](const shared_ptr<Objects>& o)
						{
							return o->getID() == FeedId;
						});
					shared_ptr<Feed> feed;
					if (itFeed == objects.end())
					{
						feed = make_shared<Feed>(Vector2f(f["Center"]["x"], f["Center"]["y"]), f["Radius"], f["id"]);
						feed->setSpeed(Vector2f(f["Speed"]["x"], f["Speed"]["y"]));
						tempObjects.push_back(feed);
					}
					else
					{
						feed = dynamic_pointer_cast<Feed>(*itFeed);
						if (feed)
						{
							feed->setSpeed(Vector2f(f["Speed"]["x"], f["Speed"]["y"]));
							feed->setDifference(Vector2f(f["Center"]["x"], f["Center"]["y"]) - itFeed->get()->getCenter());
							tempObjects.push_back(feed);
						}
					}
				}*/
			}
				
			if (response.contains("listFood"))
			{

				for (const auto& f : response["listFood"])
				{
					if (f["state"] != 0)
						continue;
					int FoodId = static_cast<int>(f["id"]);
					auto itFood = find_if(objects.begin(), objects.end(), [FoodId](const shared_ptr<Objects>& o)
						{
							return o->getID() == FoodId;
						});
					shared_ptr<Food> food;
					if (itFood == objects.end())
					{
						food = make_shared<Food>(Vector2f(f["Center"]["x"], f["Center"]["y"]),f["Radius"],f["id"]);
						food->setColor(f["Color"]);
							tempObjects.push_back(food);
					}
					else
					{
						food = dynamic_pointer_cast<Food>(*itFood);
						if (food)
						{
							tempObjects.push_back(food);
						}
					}
				}
			}

			if (response.contains("listFeed"))
			{

				for (const auto& f : response["listFeed"])
				{
					if (f["state"] != 0)
						continue;
					int FeedId = static_cast<int>(f["id"]);
					auto itFeed = find_if(objects.begin(), objects.end(), [FeedId](const shared_ptr<Objects>& o)
						{
							return o->getID() == FeedId;
						});
					shared_ptr<Feed> feed;
					if (itFeed == objects.end())
					{
						feed = make_shared<Feed>(Vector2f(f["Center"]["x"], f["Center"]["y"]), f["Radius"], f["id"]);
						feed->setSpeed(Vector2f(f["Speed"]["x"], f["Speed"]["y"]));
						tempObjects.push_back(feed);
					}
					else
					{
						feed = dynamic_pointer_cast<Feed>(*itFeed);
						if (feed)
						{
							feed->setSpeed(Vector2f(f["Speed"]["x"], f["Speed"]["y"]));
							feed->setDifference(Vector2f(f["Center"]["x"], f["Center"]["y"]) - itFeed->get()->getCenter());
							tempObjects.push_back(feed);
						}
					}
				}
			}

			if (response.contains("listThorn"))
			{	
				for (const auto& t : response["listThorn"])
				{
					if (t["state"] != 0)
						continue;
					int ThornId = static_cast<int>(t["id"]);
					auto itThorn = find_if(objects.begin(), objects.end(), [ThornId](const shared_ptr<Objects>& o)
						{
							return o->getID() == ThornId;
						});
					shared_ptr<Thorn> thorn;
					if (itThorn == objects.end())
					{
						thorn = make_shared<Thorn>(Vector2f(t["Center"]["x"], t["Center"]["y"]), t["Radius"], t["id"]);
						tempObjects.push_back(thorn);
					}
					else
					{
						thorn = dynamic_pointer_cast<Thorn>(*itThorn);
						if (thorn)
							tempObjects.push_back(thorn);
					}
				}
			}
			if (response.contains("listBot"))
			{
				for (const auto& obj : response["listBot"])
				{
					if (obj["state"] != 0)
						continue;
					int botId = static_cast<int>(obj["id"]);
					auto itBot = find_if(objects.begin(), objects.end(), [botId](const shared_ptr<Objects>& o) 
						{ 
							return o->getID() == botId;
						});
					shared_ptr<Bot> bot;
					if (itBot == objects.end())
					{
						bot = make_shared<Bot>(Vector2f(obj["Center"]["x"], obj["Center"]["y"]), obj["Radius"], obj["Color"], obj["Name"], obj["id"]);
						bot->isSplitted(obj["Splitted"]);
						bot->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
						tempObjects.push_back(bot);
					}
					else
					{
						bot = dynamic_pointer_cast<Bot>(*itBot);
						if (bot)
						{
							bot->isSplitted(obj["Splitted"]);
							bot->setSpeed(Vector2f(obj["Speed"]["x"], obj["Speed"]["y"]));
							bot->setRadius(obj["Radius"]);
							bot->setDifference(Vector2f(obj["Center"]["x"], obj["Center"]["y"]) - bot->getCenter());
							tempObjects.push_back(bot);
						}
					}
					if (obj["Splitted"] == true)
					{
						for (auto& p : obj["Pieces"])
						{	
							int pieceId = static_cast<int>(p["id"]);
							auto itPiece = find_if(objects.begin(), objects.end(), [pieceId](const shared_ptr<Objects>& o)
								{
									return o->getID() == pieceId;
								});
							shared_ptr<Piece> piece;
							if (itPiece == objects.end())
							{
								piece = make_shared<Piece>(Vector2f(p["Center"]["x"], p["Center"]["y"]), p["Radius"], p["Color"], p["id"]);
								piece->setSpeed(Vector2f(p["Speed"]["x"], p["Speed"]["y"]));
								piece->setMaxV(p["maxV"]);
								tempBotPieces.push_back(piece);
								tempObjects.push_back(piece);
							}
							else
							{
								piece = dynamic_pointer_cast<Piece>(*itPiece);
								if (piece)
								{
									piece->setMaxV(p["maxV"]);
									piece->setSpeed(Vector2f(p["Speed"]["x"], p["Speed"]["y"]));
									piece->setRadius(p["Radius"]);
									auto it = find_if(bot->pieces.begin(),bot->pieces.end(), [piece](const shared_ptr<Piece>& pTemp)
										{
											return pTemp->getID() == piece->getID();
										});
									if (it != bot->pieces.end())
									{
										piece->setDifference(Vector2f(p["Center"]["x"], p["Center"]["y"]) - it->get()->getCenter());
									}
									tempObjects.push_back(piece);
									tempBotPieces.push_back(piece);
								}
							}
						}
						bot->pieces = tempBotPieces;
						tempBotPieces.clear();
					}
				}
			}
			objects = tempObjects;
			tempObjects.clear();

			sort(objects.begin(), objects.end(), [](const shared_ptr<Objects> & obj1, const shared_ptr<Objects> & obj2)
				{
					return obj1->getRadius() < obj2->getRadius();
				});
			if (response.contains("Hero") && response["Hero"]["GameOver"])
			{
				GameOver = true;
			}
		}
		this_thread::sleep_for(chrono::milliseconds(300));
	}
	
}
GameScene::GameScene(SceneManager* manager): _manager(manager), _hero(nullptr)
{

	text.setString(L"Вы Проиграли");
	text.setFont(*rec.font);
	text.setCharacterSize(80);
	text.setFillColor(Color::Red);

	HeroMass.setFont(*rec.font);
	HeroMass.setCharacterSize(28);
	HeroMass.setPosition(20, 50);
	HeroMass.setFillColor(Color::Black);
	

	_GameOver = false;

	
	
 	fieldBGSprite.setTexture(*rec.textures[ResourceManager::Textures::FIELD]);

	thread t(requestObjects, ref(objects), ref(HeroMass), ref(_GameOver), ref(scale), ref(CameraPos), ref(_hero));
	t.detach();
	

}

void sendMessage(nlohmann::json request)
{
	nlohmann::json response = message.process(request);
}

void GameScene::HandleEvent(Event& event, Vector2f& mousePos)
{

	nlohmann::json request;
	if (event.type == sf::Event::KeyPressed)
	{
		
		if (event.key.scancode == sf::Keyboard::Scan::Escape)
		{
			_manager->SetMainScene();
		}
		if (event.key.code == Options::SplitedKeyCode)	
		{
			request["action"] = "split";
			nlohmann::json response = message.process(request);
			
		}
		if (event.key.code == Options::FeededKeyCode)
		{		
			request["action"] = "feed";
			nlohmann::json response = message.process(request);
		}
	}
	if (event.type == sf::Event::MouseMoved)
	{
		posMouse = mousePos;
		
		if(_hero != nullptr)
			_hero->setPosMouse(mousePos.x, mousePos.y);
	}
	
}

void GameScene::TimeElapsed(int& diff)
{
	timer += diff;
	if (timer > 100)
	{
		nlohmann::json request;
		request["action"] = "ask pos mouse";
		request["mousePosition"] = { {"x",posMouse.x}, {"y", posMouse.y} };
		nlohmann::json response = message.process(request);
	}
	lock_guard lock(m);
	for (const auto& o : objects)
	{
		o->TimeElapsed(diff);
	}
	
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

void GameScene::draw(RenderWindow& window)
{	
	lock_guard lock(m);
	window.draw(fieldBGSprite);	
	
	for (auto& obj : objects)
	{
		obj->draw(window);
	}		
	window.draw(HeroMass);

	if(_GameOver)
	{
		text.setPosition(Options::_WindowWidth / 2 - text.getGlobalBounds().width / 2, Options::_WindowHeight/2 - text.getGlobalBounds().height / 2);
		window.draw(text);
	}
}
