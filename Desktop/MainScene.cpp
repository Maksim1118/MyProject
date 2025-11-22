#include "MainScene.h"
#include "ButtonBackGround.h"
#include "SceneManager.h"
#include <iostream>

constexpr int timerCursorBlink = 500;
constexpr int MAX_LENGTH_NAME = 15;

MainScene::MainScene(SceneManager* manager)
{
    _manager = manager;   
    current = nullptr;

    BackgroundSprite.setTexture(*rec.textures[ResourceManager::Textures::MENU]);
}

void MainScene::HandleEvent(Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
       if (button1.isPoz(m_MousePos.x, m_MousePos.y))
       {
           current = &button1;
       }
       else if (button2.isPoz(m_MousePos.x, m_MousePos.y))
       {
           current = &button2;
       }
       else if (button3.isPoz(m_MousePos.x, m_MousePos.y))
       {
           current = &button3;
       }
       else
       {
           current = nullptr;
       }
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        if (current == &button2)
        {
            m_currSubScene = move(createSettingScene());
        }
    }

    if (m_currSubScene)
    {
        m_currSubScene->HandleEvent(event, m_MousePos);
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      if (current == &button1)
      {
          _manager->SetGameScene("jkfjg");
      }
    }
}
void MainScene::TimeElapsed(int& diff)
{
    if (m_currSubScene)
    {
        m_currSubScene->TimeElapsed(diff, m_MousePos);
        if (m_currSubScene->shouldClose())
        {
            m_currSubScene.reset();
        }
    }
}
MainScene::NickNameInput* MainScene::createInputName()
{
    return new NickNameInput();
    
}

void MainScene::deleteInputName()
{

}

unique_ptr<SubScene> MainScene::createSettingScene()
{
    try
    {
        return make_unique<SettingScene>();
    }
    catch (std::bad_alloc& e)
    {
        cerr << "error create setting sub scene: " << e.what();
        exit(-1);
    }
}

void MainScene::deleteSettingScene()
{

}

void MainScene::lockButtons()
{
    button1.setActive(false);
    button2.setActive(false);
    button3.setActive(false);
    current = nullptr;
}

void MainScene::unlockButtons()
{
    button1.setActive(true);
    button2.setActive(true);
    button3.setActive(true);
}


void MainScene::draw(RenderWindow& window)
{

    window.draw(BackgroundSprite);
    if (current)
    {
        current->drawBG(window);
    }
    button1.draw(window);
    button2.draw(window);
    button3.draw(window);
    if (m_currSubScene)
    {
        m_currSubScene->draw(window);
    }
}

MainScene::NickNameInput::NickNameInput()
{
    BackGroundInputField.setTexture(*rec.textures[ResourceManager::Textures::BACKGROUNDINPUT]);
    BackGroundInputField.setScale(0.7f, 0.7f);
    
    Vector2u bgTextureSize = rec.textures[ResourceManager::Textures::BACKGROUNDINPUT]->getSize();
    BackGroundInputField.setOrigin(bgTextureSize.x / 2.f, bgTextureSize.y / 2.f);
    BackGroundInputField.setPosition(1200 / 2.f, 1200 / 2.f);


    InputField.setTexture(*rec.textures[ResourceManager::Textures::INPUTFIELD]);
    Vector2u inputTextureSize = rec.textures[ResourceManager::Textures::INPUTFIELD]->getSize();
    float scaleX = (0.7f * 0.8f * bgTextureSize.x) / inputTextureSize.x;
    float scaleY = (0.7f * 0.25f * bgTextureSize.y) / inputTextureSize.y;
    InputField.setScale(scaleX, scaleY);

    InputField.setOrigin(inputTextureSize.x / 2.f, inputTextureSize.y / 2.f);
    InputField.setPosition(BackGroundInputField.getPosition());

    FloatRect inputBounds = InputField.getGlobalBounds();
    unsigned int charSize = static_cast<unsigned int>(inputBounds.height * 0.65f);

    text.setFont(*rec.font);
    text.setCharacterSize(charSize);
    text.setString("Enter nickname...");
    text.setFillColor(sf::Color(150, 150, 150));

    float textPosX = inputBounds.left + 0.03f * inputBounds.width;
    float textPosY = inputBounds.top + (inputBounds.height - (float)text.getCharacterSize())/2.f;
    text.setPosition(textPosX, textPosY);

    isActive = false;
    cursorVisible = false;
    cursorBlinkTime = 0.f;
    buffer = "";
    prevBufferSize = buffer.size();
}

void MainScene::NickNameInput::draw(sf::RenderTarget& target)
{
    target.draw(BackGroundInputField);
    target.draw(InputField);
    target.draw(text);

    if (isActive && cursorVisible)
    {
        target.draw(cursor);
    }
}

void MainScene::NickNameInput::handleEvent(Event& event, const Vector2f& mousePos)
{
    if (event.type == Event::MouseButtonPressed)
    {
        if (!isActive)
        {
            isActive = InputField.getGlobalBounds().contains(mousePos);
            if (isActive)
            {
                text.setString("");
                setCursor();
            }
        }
    }

    else if (isActive && event.type == Event::TextEntered)
    {
        visibleCursorLock();
        if (event.text.unicode == '\b')
        {
            if (!buffer.empty())
            {
                buffer.pop_back();
                text.setString(buffer);
            }
            else if (buffer.empty())
            {
                text.setString("");
            }
        }

        else if (event.text.unicode < 128 && event.text.unicode != '\r'
            && event.text.unicode != '\n' && buffer.size() < MAX_LENGTH_NAME)
        {
            buffer += static_cast<char>(event.text.unicode);
            text.setString(buffer);
        }
    }
}

void MainScene::NickNameInput::TimeElapsed(int diff)
{
    updateBlinkCursor(diff);
    if (isActive && prevBufferSize != buffer.size())
    {
        prevBufferSize = buffer.size();
        updateCursorPos();
    }
}

void MainScene::NickNameInput::setCursor()
{
    cursor.setSize({ 2.f, (float)text.getCharacterSize()});
    cursor.setFillColor(Color(20.f, 20.f, 20.f));
    updateCursorPos();
    cursorVisible = true;
}

void MainScene::NickNameInput::updateBlinkCursor(int diff)
{
    if (isActive)
    {
        cursorBlinkTime += diff;
        if (cursorBlinkTime >= timerCursorBlink)
        {
            cursorVisible = !cursorVisible;
            cursorBlinkTime = 0.f;
        }
    }
}

void MainScene::NickNameInput::visibleCursorLock()
{
    cursorBlinkTime = 0.f;
    cursorVisible = true;
}

void MainScene::NickNameInput::updateCursorPos()
{
    if (!buffer.empty())
    {
        sf::Vector2f pos = text.findCharacterPos(buffer.size());
        cursor.setPosition(pos.x, pos.y);
    }
    else
    {
        cursor.setPosition(text.getPosition().x, text.getPosition().y);
    }
}

string& MainScene::NickNameInput::getString()
{
    return buffer;
}

