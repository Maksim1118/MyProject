
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SceneManager.h"
#include "ButtonBackGround.h"
#include "SettingScene.h"
#include "ResourсeManager.h"
#include "GameScene.h"
#include "Message.h"
#include "PastTime.h"

#include <iostream>
#include <thread>

using namespace sf;
using namespace std;

float propX = VideoMode::getDesktopMode().width / 1200.f;
float propY = VideoMode::getDesktopMode().height / 1200.f;
float prop = min(propX, propY);


Vector2f getViewSize()
{
    float widthRect = VideoMode::getDesktopMode().width / prop;
    float heightRect = VideoMode::getDesktopMode().height / prop;
    return Vector2f(widthRect, heightRect);
}

//bool Options::_isBoxPressed = false;
//bool Options::_isSaveActive = false;
//bool Options::_isKeySpliteChanged = false;
//bool Options::_isKeyFeedingChanged = false;
int main()
{
    setlocale(LC_ALL, "ru");
    /*Options::OpenSaved("SaveSettings.txt"); */

    ContextSettings settings;
    settings.antialiasingLevel = 12;
    RenderWindow window(VideoMode(1200, 1200), "HUNGRY BALLS", Style::Titlebar | Style::Close, settings);
 /*   Options::_WindowWidth = window.getSize().x;
    Options::_WindowHeight = window.getSize().y;*/





    rec.load();
  
    SceneManager manager;

    RectangleShape shape;
    RectangleShape square;

    shape.setSize(Vector2f(30, 30));
    shape.setFillColor(Color::Red);
    shape.setPosition(0, 0);
    square.setSize(Vector2f(30, 30));
    square.setFillColor(Color::Red);
    square.setPosition(0, 600);

    bool isFullscreen = false;
    bool isMusicPlaying = false;
    Vector2f mousePos;
    RectangleShape rect1, rect2;
   
    float k = (getViewSize().x - 1200) / 2;

    rect1.setFillColor(Color::Black);
    rect1.setSize(Vector2f(k, getViewSize().y));
    rect1.setPosition(-k, 0);


    rect2.setFillColor(Color::Black);
    rect2.setSize(Vector2f(k, getViewSize().y));
    rect2.setPosition(1200, 0);

    nlohmann::json request;
    nlohmann::json response;
    PastTime time;
    /*PastTime time2;*/
    while (window.isOpen())
    {
        /*if (!isMusicPlaying)
        {           
            rec.music->setLoop(true);         
            rec.music->play();
  
            isMusicPlaying = true;
        }       */
       /* rec.music->setVolume(Options::volume);*/
        mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        manager.GetCurrent()->updateMousePos(mousePos);
        Event event;
        while (window.pollEvent(event))
        {          
            if (event.type == sf::Event::Resized)
            {
                View view = window.getView();
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.0, event.size.height / 2.0);
                window.setView(view);
              
            }           
            manager.GetCurrent()->HandleEvent(event);

            if (event.type == Event::Closed)
            {
                window.close();
            }               
        }
        time.update();
       
        int diff = time.getDeltaTime();
       /* time2.update();*/
       /* int diffEvents = time2.getDeltaTime();*/
        manager.GetCurrent()->TimeElapsed(diff);
      /*  time2.update();
        int diffTimeElapsed = time2.getDeltaTime();*/
        window.clear();
        manager.GetCurrent()->draw(window);
        window.draw(shape);
        window.draw(square);
        if (isFullscreen)
        {
            window.draw(rect1);
            window.draw(rect2);
        }     
        window.display();   
      /*  time2.update();
        int diffDraw = time2.getDeltaTime();*/

      
    }
    rec.clear();
    return 0;
}
