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
        SETTINGPICT,
        INPUTFIELD,
        BACKGROUNDINPUT,
        SETTINGBG,
        BUTTONCLOSE,
        BUTTONCLOSEPRESSED,
        BOX,
        CHECKMARK,
        TRACK,
        THUMB,
        THUMB_HOVERED,
        MARKDOWN,
        MARKUP,
        FRAME_DROPDOWN,
        BLUR_BOX,
        BUTTON_FRAME_NORM,
        BUTTON_FRAME_BLUR
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
        // to do восстановить
        /*textures[Textures::BUTTON_FRAME_NORM] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\frameButtonNormTex.png");
        textures[Textures::BUTTON_FRAME_BLUR] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\frameButtonBlurTex.png");*/
        textures[Textures::SETTINGPICT] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\setting_picture.png");
        textures[Textures::BUTTONCLOSE] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\CloseButton.png");
        textures[Textures::BUTTONCLOSEPRESSED] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\CloseButtonPressed.png");
        textures[Textures::INPUTFIELD] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\InputField.png");
        textures[Textures::BACKGROUNDINPUT] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\BackGroundInputField.png");
        textures[Textures::SETTINGBG] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\Setting.png");
        textures[Textures::BOX] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\box.png");
        textures[Textures::BLUR_BOX] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\blurBox.png");
        textures[Textures::CHECKMARK] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\check_mark.png");
        textures[Textures::TRACK] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\track.png");
        textures[Textures::THUMB] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\thumb.png");
        textures[Textures::THUMB_HOVERED] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\thumbHovered.png");
        textures[Textures::MARKDOWN] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\markDown.png");
        textures[Textures::MARKUP] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\markUp.png");
        textures[Textures::FRAME_DROPDOWN] = loadTexture("D:\\HungryBall\\out\\build\\x64-Debug\\bin\\Resources\\image\\frame.png");
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