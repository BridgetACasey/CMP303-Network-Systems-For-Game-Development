//@BridgetACasey

#include "input_manager.h"

InputManager::InputManager()
{
	mouse = new Mouse();
}

InputManager::~InputManager()
{
	if (mouse)
	{
		delete mouse;
	}
}