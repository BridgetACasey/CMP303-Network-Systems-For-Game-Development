//@BridgetACasey

#include "chat_manager.h"

ChatManager::ChatManager(InputManager* input)
{
	inputManager = input;
	last = 0;
	delay = 0.0f;
}

ChatManager::~ChatManager()
{

}

void ChatManager::updateMessageStream(float deltaTime)
{
	if (inputManager->getKeyStatus(sf::Keyboard::Key::Backspace) == InputStatus::PRESSED)
	{
		if(message.getSize() > 0)
		message.erase(message.getSize() - 1, 1);
	}

	if (message.getSize() > 64)
	{
		return;
	}

	if (inputManager->getCurrentChar())
	{
		if (inputManager->getCurrentChar() == last)
		{
			if(delay < 1.0f)
			{
				delay += (1.0f * deltaTime);
				return;
			}

		}

		else
		{
			delay = 0.0f;
		}

		message.insert(message.getSize(), sf::String(inputManager->getCurrentChar()));
		last = inputManager->getCurrentChar();
	}

	else
	{
		last = 0;
	}
}