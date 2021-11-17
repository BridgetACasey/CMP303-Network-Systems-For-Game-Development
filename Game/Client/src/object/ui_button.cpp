//@BridgetACasey

#include "ui_button.h"

UIButton::UIButton(InputManager* input, const char* active, const char* idle)
{
	inputManager = input;

	idleTexture = new sf::Texture();
	activeTexture = new sf::Texture();

	if (!activeTexture->loadFromFile(active))
	{
		printf("could not load texture");
	}

	if (!idleTexture->loadFromFile(idle))
	{
		printf("could not load texture");
	}

	setTexture(idleTexture);
}

UIButton::~UIButton()
{

}

bool UIButton::isHovering()
{
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