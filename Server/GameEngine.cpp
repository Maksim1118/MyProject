#include "GameEngine.h"
#include "BaseOld.h"
#include "HeroServer.h"
#include "MessageServer.h"

#include <ctime>
#include<iostream>

constexpr int fieldWidth = 1200;
constexpr int fieldHeight = 1200;
constexpr float maxMassForScale = 4000.00f;
constexpr float minMassForScale = 400.00f;
constexpr float maxScale = 1.00f;
constexpr float minScale = 2.00f;

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

	GameEngine::GameEngine() : hero(m_ListFeeds, "myHero")
	{
		_GameOver = false;
		srand(time(NULL));

		fillList(bots, 1, "Bot");
		fillList(_ThornSprite, ThornSpriteSize, "");
		fillList(_Food, ArrFoodSize, "");

		shift(&hero);

		spawnList(bots);
		spawnList(_ThornSprite);
		spawnList(_Food);
	}

	template<typename T> void GameEngine::spawnList(list<shared_ptr<T>>& _list)
	{
		for (auto& i = _list.begin(); i != _list.end(); i++)
		{
			shift((*i).get());
			while(checkAllCollision(*i->get()))
			{
				shift((*i).get());
			}
		}
	}

	nlohmann::json GameEngine::getInformationHero(Hero& hero)
	{
		nlohmann::json Hero;
		auto heroCol = static_cast<int>(hero.colB);
		Hero["id"] = hero.getID();
		Hero["Center"] = { {"x", hero.getCenter().x},{ "y", hero.getCenter().y} };
		Hero["Radius"] = hero.getRadius();
		Hero["Color"] = heroCol;
		Hero["Name"] = hero.getName();
		Hero["Splitted"] = hero.isSplitted();
		Hero["Feeded"] = hero.isFeeded();
		Hero["Mass"] = hero.getMass();
		Hero["GameOver"] = _GameOver;
		Hero["Speed"]["x"] = hero.getSpeed().x;
		Hero["Speed"]["y"] = hero.getSpeed().y;
		Hero["State"] = static_cast<int>(hero.state);

		if (Hero["Splitted"] == true)
		{
			nlohmann::json HeroPieceArray = nlohmann::json::array();
			for (auto& p : hero.pieces)
			{
				nlohmann::json Piece;
				Piece["id"] = p->getID();
				Piece["Center"] = { {"x", p->getCenter().x},{ "y", p->getCenter().y} };
				Piece["Radius"] = p->getRadius();
				Piece["Color"] = heroCol;
				Piece["Speed"]["x"] = p->getSpeed().x;
				Piece["Speed"]["y"] = p->getSpeed().y;
				Piece["maxV"] = p->getMaxV();

				HeroPieceArray.push_back(Piece);
			}
			Hero["Pieces"] = HeroPieceArray;
		}
		return Hero;
	}



	nlohmann::json GameEngine::process(nlohmann::json request)
	{
		nlohmann::json response;
		if (request.contains("action") && request["action"] == "create Hero")
		{

			response["Hero"] = getInformationHero(hero);
			response["status"] = "OK";
		}
		if (request.contains("action") && request["action"] == "split")
		{
			hero.setSplite();
			response["status"] = "OK";
		}
		else if (request.contains("action") && request["action"] == "feed")
		{
			hero.setFeeded();
			response["status"] = "OK";
		}
		else if (request.contains("action") && request["action"] == "get mass")
		{
			response["massHero"] = hero.getMass();
		}
		else if (request.contains("action") && request["action"] == "ask pos mouse")
		{
			float x = request["mousePosition"]["x"];
			float y = request["mousePosition"]["y"];
			hero.setPosMouse(x, y);
			response["status"] = "OK";
		}
		else if (request.contains("action") && request["action"] == "get object")
		{
			nlohmann::json BotsArray = nlohmann::json::array();
			for (auto& b : bots)
			{
				auto botCol = static_cast<int>(b->colB);
				nlohmann::json Bot;
				Bot["id"] = b->getID();
				Bot["Center"] = { {"x", b->getCenter().x},{ "y", b->getCenter().y} };
				Bot["Radius"] = b->getRadius();
				Bot["Color"] = botCol;
				Bot["Name"] = b->getName();
				Bot["Splitted"] = b->isSplitted();
				Bot["Speed"]["x"] = b->getSpeed().x;
				Bot["Speed"]["y"] = b->getSpeed().y;
				Bot["Mouse"]["x"] = b->getMouse().x;
				Bot["Mouse"]["y"] = b->getMouse().y;
				Bot["state"] = static_cast<int>(b->state);
				if (Bot["Splitted"] == true)
				{
					nlohmann::json BotPieceArray = nlohmann::json::array();
					for (auto& p : b->pieces)
					{
						nlohmann::json Piece;
						Piece["id"] = p->getID();
						Piece["Center"] = { {"x", p->getCenter().x},{ "y", p->getCenter().y} };
						Piece["Radius"] = p->getRadius();
						Piece["Color"] = botCol;
						Piece["Speed"]["x"] = p->getSpeed().x;
						Piece["Speed"]["y"] = p->getSpeed().y;
						Piece["maxV"] = p->getMaxV();

						BotPieceArray.push_back(Piece);
					}
					Bot["Pieces"] = BotPieceArray;
				}
				BotsArray.push_back(Bot);
			}
			response["listBot"] = BotsArray;

			response["Hero"] = getInformationHero(hero);


			nlohmann::json FeedArray = nlohmann::json::array();
			for (const auto& f : m_ListFeeds)
			{
				nlohmann::json Feed;
				Feed["Center"] = { {"x", f->getCenter().x},{ "y", f->getCenter().y} };
				Feed["Radius"] = f->getRadius();
				Feed["id"] = f->getID();
				Feed["state"] = static_cast<int>(f->state);
				Feed["Speed"]["x"] = f->getSpeed().x;
				Feed["Speed"]["y"] = f->getSpeed().y;

				FeedArray.push_back(Feed);
			}
			response["listFeed"] = FeedArray;

			/*response["Hero"] = Hero;*/

			nlohmann::json FoodArray = nlohmann::json::array();
			for (auto& f : _Food)
			{
				nlohmann::json Food;
				Food["Center"] = { {"x", f->getCenter().x},{ "y", f->getCenter().y} };
				Food["Radius"] = f->getRadius();
				Food["Color"] = f->colorNum;
				Food["id"] = f->getID();
				Food["state"] = static_cast<int>(f->state);
				FoodArray.push_back(Food);
			}
			response["listFood"] = FoodArray;

			nlohmann::json ThornArray = nlohmann::json::array();
			for (auto& t : _ThornSprite)
			{
				nlohmann::json Thorn;
				Thorn["Center"] = { {"x", t->getCenter().x},{ "y", t->getCenter().y} };
				Thorn["Radius"] = t->getRadius();
				Thorn["id"] = t->getID();
				Thorn["state"] = static_cast<int>(t->state);
				ThornArray.push_back(Thorn);
			}
			response["listThorn"] = ThornArray;

			response["status"] = "OK";
		}
		else
		{
			response["status"] = "ERROR";
		}
		return response;
	}

	bool GameEngine::isCollWithMap(const  Vector2f& pos)
	{
		return (pos.x < 0) || (pos.x > fieldWidth) || (pos.y < 0) || (pos.y > fieldHeight) ? true : false;
	}

	Vector2f GameEngine::getCoorCollWithMap(const Vector2f& pos)
	{
		if (pos.x < 0)
			return Vector2f(0, pos.y);
		else if (pos.x > fieldWidth)
			return Vector2f(fieldWidth, pos.y);
		else if (pos.y < 0)
			return  Vector2f(pos.x, 0);
		else if (pos.y > fieldHeight)
			return Vector2f(pos.x, fieldHeight);
	}

	void GameEngine::TimeElapsed(int& diff)
	{
		if (hero.state == States::EATEN)
			_GameOver = true;
		if (!_GameOver)
			hero.TimeElapsed(diff);
		checkStatus(bots, diff);
		checkStatus(_Food, diff);
	
		cout << "begin" << "   ";
		checkStatus(m_ListFeeds, diff);
		cout << "   " << "end";
		checkStatus(_ThornSprite, diff);

		allObjectsCollWithMap();
		calcDirMoveBot(diff);
		
		for (auto& i = _Food.begin(); i != _Food.end(); ++i)
		{
			hero.Eat((*i).get());
		}

		for (auto& i = _Food.begin(); i != _Food.end(); ++i)
		{
			for (auto j = bots.begin(); j != bots.end(); j++)
			{
				(*j)->Eat((*i).get());
			}
		}

		for (auto& i = m_ListFeeds.begin(); i != m_ListFeeds.end(); ++i)
		{
			hero.Eat((*i).get());
		}

		for (auto& i = m_ListFeeds.begin(); i != m_ListFeeds.end(); ++i)
		{
			for (auto j = bots.begin(); j != bots.end(); j++)
			{
				(*j)->Eat((*i).get());
			}
		}

		for (auto& i = _ThornSprite.begin(); i != _ThornSprite.end(); ++i)
		{
			hero.Eat((*i).get());
		}

		for (auto& i = _ThornSprite.begin(); i != _ThornSprite.end(); ++i)
		{
			for (auto& b : bots)
			{
				b->Eat((*i).get());
			}
		}

		for (auto& i = bots.begin(); i != bots.end(); ++i)
		{
			if ((*i)->state == States::EATEN)
			{
				continue;
			}
			for (auto j = bots.begin(); j != bots.end(); ++j)
			{
				if (i == j) continue;
				(*i)->Eat((*j).get());
			}
		}

		if (!_GameOver)
		{
			for (auto& b : bots)
			{
				b->Eat(&hero);
				hero.Eat(b.get());
			}		
		}
	}

	template<typename T> void GameEngine::shift(T* ptrObj)
	{
		float x = (float)(rand() % fieldWidth);
		float y = (float)(rand() % fieldHeight);
		ptrObj->setCenter(x, y);
	}

	template<typename T> void GameEngine::fillList(list<shared_ptr<T>>& list, size_t size, string str)
	{
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				shared_ptr<T> obj;
				if constexpr (is_same_v<T, Bot>)
				{
					obj = make_shared<T>(str + to_string(i + 1));
				}
				else
				{
					obj = make_shared<T>();
				}
				if (obj)
				{
					list.push_back(obj);
				}
			}
		}
	}

	void GameEngine::allObjectsCollWithMap()
	{
		if (isCollWithMap(hero.getCenter()))
			hero.setCenter(getCoorCollWithMap(hero.getCenter()));

		for (auto& f : m_ListFeeds)
		{
			if (isCollWithMap(f->getCenter()))
				f->setCenter(getCoorCollWithMap(f->getCenter()));
		}
		for (auto& b : bots)
		{
			if (isCollWithMap(b->getCenter()))
				b->setCenter(getCoorCollWithMap(b->getCenter()));
		}
		for (auto& Piece : hero.pieces)
		{
			if (isCollWithMap(Piece->getCenter()))
				Piece->setCenter(getCoorCollWithMap(Piece->getCenter()));
		}
	}

	template<typename T> void GameEngine::checkStatus(list<shared_ptr<T>>& _list, int diff)
	{
		for (auto& i = _list.begin(); i != _list.end(); i++)
		{
			(*i)->TimeElapsed(diff);
			if ((*i)->state == States::EATEN)
			{
				continue;
			}
			if ((*i)->state == States::READY_TO_LIVE)
			{
				updatePosAfterEaten((*i), _list);
				(*i)->state = States::LIVE;
			}
		}
	}

	void GameEngine::calcDirMoveBot(int diff)
	{
		for (auto& i = bots.begin(); i != bots.end(); i++)
		{
			if ((*i)->state == States::EATEN)
			{
				continue;
			}
			Vector2f to(0.0, 0.0);
			for (auto& j = _Food.begin(); j != _Food.end(); ++j)
			{
				if ((*j)->state == States::EATEN)
					continue;
				to += calcDistBotAndObj((**i), (**j), (*i)->getRadius() * 0.99f);
			}
			{
				for (auto& j = _ThornSprite.begin(); j != _ThornSprite.end(); ++j)
				{
					to -= 4.f * calcDistBotAndObj((**i), (**j), min((*i)->getRadius(), (*j)->getRadius()) * 0.99f);
				}
			}
			to = getIdentityVector(to) * 100.f + (*i)->getCenter();
			(*i)->setPosMouse(to.x, to.y);
			(*i)->TimeElapsed(diff);
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

	template<typename T> void GameEngine::updatePosAfterEaten(shared_ptr<T>& ptrObj, list<shared_ptr<T>>& listPtrObjects)
	{
		bool collision = true;
		while (collision)
		{
			shift(ptrObj.get());
			collision = checkAllCollision(*ptrObj.get());
		}
	}

	template<typename T> bool GameEngine::checkCollision(Objects* obj, list<shared_ptr<T>>& _list)
	{
		for (auto& j : _list)
		{
			if (obj != j.get() && obj->isCollision(*j, 2))
			{
				return true;
			}
		}
		return false;
	}

	bool GameEngine::checkAllCollision(Objects& obj)
	{
		return (obj.isCollision(hero, 2) || checkCollision(&obj, bots) || checkCollision(&obj, _ThornSprite) || checkCollision(&obj, _Food));

	}
}
