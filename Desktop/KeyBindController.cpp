#include "KeyBindController.h"

#include "Options.h"
#include "CharButton.h"

using namespace sf;
using namespace std;

KeyBindController::KeyBindController()
	:m_charButton(nullptr), m_isActive(false)
{
}

KeyBindController::KeyBindController(KeyBindController&& other) noexcept
	:m_charButton(exchange(other.m_charButton,nullptr))
	, m_isActive(exchange(other.m_isActive, false))
{

}

KeyBindController& KeyBindController::operator=(KeyBindController&& other) noexcept
{
	if (this != &other) {
		m_charButton = exchange(other.m_charButton, nullptr);
		m_isActive = exchange(other.m_isActive, false);
	}
	return *this;
}

void KeyBindController::handleEvent(const Event& event)
{
    if (!m_charButton)
    {
        unBind(); 
        return;
    }

    if (event.type == Event::TextEntered)
    {
        if (event.text.unicode == 27 || event.text.unicode == 13) //Escape || Enter
        {
            if (m_charButton) {
                m_charButton->setKeyCode();
            }
            unBind();
        }
        else
        {
            if (m_charButton) {
                m_charButton->setKeyCode(event.text.unicode);
            }
            unBind();
        }
    }
}

void KeyBindController::Bind(CharButton* charButton)
{
    if (m_isActive || !charButton) return;

    m_isActive = true;
    m_charButton = charButton;
    m_charButton->isBindingProcess = true;

    m_charButton->resetText();
}


void KeyBindController::unBind()
{
    if (m_charButton)
    {
        m_charButton->isBindingProcess = false;
    }
	m_charButton = nullptr;
	m_isActive = false;
}
