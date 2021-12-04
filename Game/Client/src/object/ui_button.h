//@BridgetACasey

#pragma once

#include "game_object.h"
#include "input/input_manager.h"
#include "SFML/Graphics/Texture.hpp"

//A button object used to navigate between states or alter the current game state
class UIButton : public GameObject
{
public:
	UIButton(InputManager* input, const char* active, const char* idle);
	~UIButton();

	bool isHovering();
	bool isClicked();

private:
	InputManager* inputManager;

	sf::Texture* activeTexture;	//The current texture if the cursor is passing over the button object
	sf::Texture* idleTexture;	//The current texture if the cursor is away from the button object
};