//@BridgetACasey

#include "ui_button.h"

UIButton::UIButton(InputManager* input)
{
	inputManager = input;
}

UIButton::~UIButton()
{

}

bool UIButton::isHovering() const
{
	float left = getPosition().x - (getSize().x / 2.0f);
	float right = getPosition().x + (getSize().x / 2.0f);
	float top = getPosition().y - (getSize().y / 2.0f);
	float bottom = getPosition().y + (getSize().y / 2.0f);

	if (inputManager->getMousePosition().x > left && inputManager->getMousePosition().x < right)
	{
		if (inputManager->getMousePosition().y > top && inputManager->getMousePosition().y < bottom)
		{
			return true;
		}
	}

	return false;
}

bool UIButton::isClicked() const
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