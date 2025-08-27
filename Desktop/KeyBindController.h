#pragma once
#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

class CharButton;
class KeyBindController
{
public:
	explicit KeyBindController();

	KeyBindController(const KeyBindController&) = delete;
	KeyBindController& operator=(const KeyBindController&) = delete;

	KeyBindController(KeyBindController&&) noexcept;
	KeyBindController& operator=(KeyBindController&&) noexcept;

	void handleEvent(const sf::Event& event);
	void Bind(CharButton* charButton);
private:
	void unBind();
private:
	CharButton* m_charButton;
	bool m_isActive;
};
