//@BridgetACasey

#include "chat_manager.h"

ChatManager::ChatManager(InputManager* input)
{
	inputManager = input;
	last = 0;
	delay = 0.0f;

	if (!arial.loadFromFile("assets/arial.ttf"))
	{
		printf("could not load font");
	}

	inputText = new sf::Text();

	inputText->setString("");
	inputText->setFont(arial);
	inputText->setCharacterSize(24);
	inputText->setFillColor(sf::Color::White);
	inputText->setPosition(sf::Vector2f(35.0f, 615.0f));
}

ChatManager::~ChatManager()
{

}

void ChatManager::updateMessageStream(float deltaTime)
{
	if (inputManager->getKeyStatus(sf::Keyboard::Key::Backspace) == InputStatus::PRESSED)
	{
		inputManager->setKeyStatus(sf::Keyboard::Key::Backspace, InputStatus::NONE);

		if(inputMessage.getSize() > 0)
		inputMessage.erase(inputMessage.getSize() - 1, 1);
	}

	if (inputMessage.getSize() > 48)
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

		inputMessage.insert(inputMessage.getSize(), sf::String(inputManager->getCurrentChar()));
		last = inputManager->getCurrentChar();
	}

	else
	{
		last = 0;
	}

	inputText->setString(inputMessage);
}

void ChatManager::addNewMessage(sf::String& name, sf::String& msg)
{
	inputMessage = sf::String("");

	sf::Text message;

	message.setString(sf::String("[" + name + "]: " + msg));
	message.setFont(arial);
	message.setCharacterSize(24);
	message.setFillColor(sf::Color(192, 192, 192, 255));
	message.setPosition(sf::Vector2f(inputText->getPosition().x, inputText->getPosition().y - 30.0f));

	chatMessages.push_back(message);

	for (sf::Text& chat : chatMessages)
	{
		chat.setPosition(sf::Vector2f(chat.getPosition().x, chat.getPosition().y - 30.0f));
	}
}