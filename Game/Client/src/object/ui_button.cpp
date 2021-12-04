//@BridgetACasey

#include <iostream>

#include "ui_button.h"

UIButton::UIButton(InputManager* input, const char* active, const char* idle)
{
	inputManager = input;

	idleTexture = new sf::Texture();
	activeTexture = new sf::Texture();

	if (!activeTexture->loadFromFile(active))
	{
		std::cout << "could not load active texture" << std::endl;
	}

	if (!idleTexture->loadFromFile(idle))
	{
		std::cout << "could not load idle texture" << std::endl;
	}

	setTexture(idleTexture);
}

UIButton::~UIButton()
{
	if (activeTexture)
	{
		delete activeTexture;
	}

	if (idleTexture)
	{
		delete idleTexture;
	}
}

//Returns true if the mouse cursor is making contact with the button object
bool UIButton::isHovering()
{
	//The bounds of the ui button object
	float left = getPosition().x;
	float right = getPosition().x + getSize().x;
	float top = getPosition().y;
	float bottom = getPosition().y + getSize().y;

	if (inputManager->getMousePosition().x > left && inputManager->getMousePosition().x < right)
	{
		if (inputManager->getMousePosition().y > top && inputManager->getMousePosition().y < bottom)
		{
			setTexture(activeTexture);
			return true;
		}
	}

	setTexture(idleTexture);
	return false;
}

//Returns true if the mouse cursor is both making contact with the button object and pressing the left mouse button
bool UIButton::isClicked()
{
	if (isHovering())
	{
		if (inputManager->getLeftMouseButton() == InputStatus::PRESSED)
		{
			return true;
		}
	}

	return false;
}