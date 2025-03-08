#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class Button 
{
public:
    Button(const wstring text, const float x, const float y);
    bool isPoz(const float& x, const float& y);
    void draw(RenderWindow& window);
    void drawBG(RenderWindow& window);
private:
    float _x{ 0 };
    float _y{ 0 };
    Text _title;
    Sprite _BG;
    Sprite _BG2;
    Text createTitle(const wstring& text, const float& x, const float& y);
};

