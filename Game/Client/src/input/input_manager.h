//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "game/window_manager.h"

enum class InputStatus	//The current status of a key or mouse button
{
	NONE = 0,
	PRESSED,
	HELD,
	RELEASED
};

struct Mouse
{
	sf::Vector2u position = sf::Vector2u(0, 0);
	InputStatus leftButton = InputStatus::NONE;
	InputStatus rightButton = InputStatus::NONE;
};

//Manager for handling keyboard and mouse input from the user
class InputManager
{
public:
	InputManager(WindowManager* window);
	~InputManager();

	inline void setKeyStatus(sf::Keyboard::Key key, InputStatus status) { keys[key] = status; }
	const inline InputStatus& getKeyStatus(sf::Keyboard::Key key) const { return keys[key]; }

	inline void setMousePosition(int x, int y) { mouse->position.x = x; mouse->position.y = y; }
	inline void setLeftMouseButton(InputStatus status) { mouse->leftButton = status; }
	inline void setRightMouseButton(InputStatus status) { mouse->rightButton = status; }

	inline sf::Vector2u& getMousePosition() const { return mouse->position; }
	inline InputStatus& getLeftMouseButton() const { return mouse->leftButton; }
	inline InputStatus& getRightMouseButton() const { return mouse->rightButton; }

	inline void setCurrentChar(char current) { currentCharacter = current; }
	inline char getCurrentChar() const { return currentCharacter; }

private:
	WindowManager* windowManager;
	
	Mouse* mouse;

	InputStatus keys[100]{ InputStatus::NONE };	//Contains a status for each of the 100 keyboard keys recognisable by SFML

	char currentCharacter;	//The unicode value of the last key that was pressed, converted to char form
};