#include "ButtonBackGround.h"
#include "ResourñeManager.h"

ButtonBackGround::ButtonBackGround(const wstring text, const float x,const float y)
{
    _x = x;
    _y = y;
    _title = createTitle(text, x, y);

    _BG.setTexture(*rec.textures[ResourceManager::Textures::BUTBG]);
    _BG.setPosition(x, y);
}
bool ButtonBackGround::isPoz(const float& x, const float& y)
{
    if (!isActive) return false;

    if (x < _x)
    {
        return false;
    }
    if (x > _x + 220)
    {
        return false;
    }
    if (y < _y)
    {
        return false;
    }
    if (y > _y + 68)
    {
        return false;
    }
    return true;
}
void ButtonBackGround::draw(RenderWindow& window)
{
    _title.setPosition(_x + 220 / 2 - (_title.getGlobalBounds().width) / 2, _y + 20);
    window.draw(_title);
}
void ButtonBackGround::setActive(const bool active)
{
    isActive = active;
}
void ButtonBackGround::drawBG(RenderWindow& window)
{
    window.draw(_BG);
}

Text ButtonBackGround::createTitle(const wstring& text,const float& x, const float& y)
{
    Text ButtonText;
    ButtonText.setFont(*rec.font); 
    ButtonText.setString(text);
    ButtonText.setCharacterSize(24);
    ButtonText.setFillColor(Color::Cyan);
    ButtonText.setStyle(Text::Bold);

    return ButtonText;
}
