//@BridgetACasey

#pragma once

#include "game_object.h"
#include "input/input_manager.h"
#include "SFML/Graphics/Texture.hpp"

class UIButton : public GameObject
{
public:
	UIButton(InputManager* input, const char* active, const char* idle);
	~UIButton();

	bool isHovering();
	bool isClicked();

private:
	InputManager* inputManager;

	sf::Texture* activeTexture;
	sf::Texture* idleTexture;
};