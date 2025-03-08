#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace sf;
using namespace std;

class  ResourceManager
{
public:
    enum  class Textures
    {
        MENU,
        FIELD,
        THORN,
        BUTBG,
        BUTCLOSE,
        SETTINGPICT
    };
    map<Textures, Texture*> textures;
    Font* font{};
    Music* music{};

    void load()
    {
        textures[Textures::MENU] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\bgmenu.png");
        textures[Textures::FIELD] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\fieldBG.png");
        textures[Textures::THORN] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\thorn2.png");
        textures[Textures::BUTBG] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\BGButton.png");
        textures[Textures::SETTINGPICT] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\setting_picture.png");
        textures[Textures::BUTCLOSE] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\ButtonClose.png");
        font = loadFont("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\font\\ofont.ru_Europe.ttf");
        music = loadMusic("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\music\\sound2.wav");    
    }

    void clear()
    {
        for (auto& pair : textures)
        {
            if (pair.second != nullptr)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        textures.clear();
        delete font;
        font = nullptr;
        delete music;
        music = nullptr;
    }

    ~ResourceManager();
 
private:
    Texture* loadTexture(const string& filename)
    {
        Texture* texture = new Texture();
        if (!texture ->loadFromFile(filename))
        {
            throw  runtime_error("Не удалось загрузить текстуру из файла " + filename);
        }
        return  texture;
    }

    Font* loadFont(const string& filename)
    {
        Font* font = new Font();
        if(!font->loadFromFile(filename))
            throw  runtime_error("Не удалось загрузить шрифт из файла " + filename);
        return font;
    }

    Music* loadMusic(const string& filename)
    {
        Music* music = new Music();
        if (!music->openFromFile(filename))
            throw  runtime_error("Не удалось загрузить музыку из файла " + filename);
        return music;
    }
};
extern  ResourceManager rec;