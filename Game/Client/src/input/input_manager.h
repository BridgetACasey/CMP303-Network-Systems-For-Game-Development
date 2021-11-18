//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "game/window_manager.h"

enum class InputStatus
{
	NONE = 0,
	PRESSED,
	HELD,
	RELEASED
};

struct Mouse
{
	sf::Vector2u position;
	InputStatus leftButton, rightButton;
};

class InputManager
{
public:
	InputManager(WindowManager* window);
	~InputManager();

	void update(float deltaTime);

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

	InputStatus keys[100]{ InputStatus::NONE };

	char currentCharacter;
};