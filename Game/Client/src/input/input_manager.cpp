//@BridgetACasey

#include "input_manager.h"

InputManager::InputManager(WindowManager* window)
{
	windowManager = window;
	
	mouse = new Mouse();
}

InputManager::~InputManager()
{
	if (mouse)
	{
		delete mouse;
	}
}

void InputManager::update(float deltaTime)
{

}