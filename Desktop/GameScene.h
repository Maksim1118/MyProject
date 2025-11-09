#pragma once
#include <list>
#include "Scene.h"
#include "Food.h"
#include "Hero.h"
#include "Thorn.h"
#include "SettingScene.h"
#include "Objects.h"
#include "Feed.h"
#include "Piece.h"
#include "Camera.h"
//#include "GameEngine.h"
#include "ResourñeManager.h"
#include "MapSegment.h"

#include <unordered_set>
#include <vector>

constexpr int ArrFoodSize = 500;
constexpr int SetPosFood = 1190;
constexpr int SetPosThorn = 1100;
class SceneManager;

class GameScene : public Scene
{
public:
	GameScene(SceneManager* manager);
	virtual void HandleEvent(Event& event);
	virtual void draw(RenderWindow& window);
	//bool isCollWithMap(const Vector2f& pos);
	//Vector2f getCoorCollWithMap(const Vector2f& pos);
	virtual void TimeElapsed(int& diff);
	Vector2f getCoorCollCameraWithMap(Vector2f& pos, float Width, float Height);
	void setActive(const std::string& playerName);
	struct Map
	{
		std::unordered_map<std::string, Hero*> ListPlayers;
		std::unordered_map<std::string, Bot*> ListBots;
		std::unordered_map<std::string, Thorn*> ListThorns;
		std::unordered_map<std::string, Food*> ListFoods;
		std::unordered_map<std::string, Feed*> ListFeeds;
		std::unordered_map<std::string, Piece*> ListPieces;
		Hero* LocalPlayer;
		list<Objects*> objects;
		unordered_set<string> ListOldId;
		float scale;
		Vector2f CameraPos;
		Camera camera;
		sf::Vector2f size;
		Map()
			:LocalPlayer(nullptr), scale(1.0f)
		{
			
		}
		void pieceToSides();
		/*sf::Vector2f calcAttractionForcePiece(Piece* &piece, const sf::Vector2f& mousePos);
		sf::Vector2f calcAccelerationPiece(const sf::Vector2f& F, Piece* & piece);
		sf::Vector2f calcSpeedPiece(const sf::Vector2f& a, const sf::Vector2f& mousePos, int diff, Piece* & piece);*/
		/*void updatePiecesPhysics();*/

		void fillListObjects();
		void insertInObjectsList(Objects* obj);
	};
private:
	/*void cameraUpdate(float diff);
	void settingCameraInterval();*/

	std::pair<int, int> getSymetricRange(int count) const;
private:

	Camera m_LocalCamera;
	sf::Vector2f m_segmentSize;
	sf::Vector2i m_maxSegIndices;
	segmentInterval m_cameraInterval;

	sf::Text m_MassText;
	SceneManager *_manager= nullptr;
	Sprite fieldBGSprite;
	Text text;
	Map m_Map;
	std::unordered_map<int, mapSegment> m_segmentsMap;
	int requestTimeStart = 0;
	int timer = 0;
	View m_view;
};
