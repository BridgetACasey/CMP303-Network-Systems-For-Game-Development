//@BridgetACasey

#pragma once

#include "game_object.h"
#include "input/input_manager.h"

class UIButton : public GameObject
{
public:
	UIButton(InputManager* input);
	~UIButton();

	bool isHovering() const;
	bool isClicked() const;

private:
	InputManager* inputManager;
};