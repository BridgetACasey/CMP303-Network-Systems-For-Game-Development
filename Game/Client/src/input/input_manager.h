//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

enum class InputStatus
{
	NONE = 0,
	PRESSED,
	HELD,
	RELEASED
};

struct Mouse
{
	sf::Vector2i position;
	InputStatus leftButton, rightButton;
};

class InputManager
{
public:
	InputManager();
	~InputManager();

	const inline void setKeyStatus(sf::Keyboard::Key& key, InputStatus& status) { keys[key] = status; }
	const inline InputStatus& getKeyStatus(sf::Keyboard::Key& key) const { return keys[key]; }

	const inline void setMousePosition(int x, int y) { mouse->position.x = x; mouse->position.y = y; }
	const inline void setLeftMouseButton(InputStatus& status) { mouse->leftButton = status; }
	const inline void setRightMouseButton(InputStatus& status) { mouse->rightButton = status; }

	const inline sf::Vector2i& getMousePosition() const { return mouse->position; }
	const inline InputStatus& getLeftMouseButton() const { return mouse->leftButton; }
	const inline InputStatus& getRightMouseButton() const { return mouse->rightButton; }

private:
	Mouse* mouse;

	InputStatus keys[100]{ InputStatus::NONE };
};