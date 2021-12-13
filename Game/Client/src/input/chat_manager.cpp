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

	MAX_MESSAGES = 15;

	MAX_INPUT = 47;
}

ChatManager::ChatManager(InputManager* input, sf::Text* inputT, const int maxInput)
{
	inputManager = input;
	last = 0;
	delay = 0.0f;

	if (!arial.loadFromFile("assets/arial.ttf"))
	{
		printf("could not load font");
	}

	inputText = inputT;

	inputText->setFont(arial);

	MAX_MESSAGES = 1;

	MAX_INPUT = maxInput;
}

ChatManager::~ChatManager()
{

}

void ChatManager::updateMessageStream(float deltaTime)
{
	if (inputManager->getKeyStatus(sf::Keyboard::Key::Backspace) == InputStatus::PRESSED)
	{
		inputManager->setKeyStatus(sf::Keyboard::Key::Backspace, InputStatus::NONE);

		//Check if stream has characters that can be erased, and delete one by one as appropriate
		if(inputMessage.getSize() > 0)
		inputMessage.erase(inputMessage.getSize() - 1, 1);
	}

	if (inputMessage.getSize() > MAX_INPUT)	//Don't update the stream if the character limit has been reached
	{
		return;
	}

	if (inputManager->getCurrentChar())
	{
		if (inputManager->getCurrentChar() == last)	//If holding down the same key, start a timer
		{
			if(delay < 1.0f)	//Increment the timer until at least one second has passed, then proceed with insertion
			{
				delay += (1.0f * deltaTime);
				return;
			}

		}

		else
		{
			delay = 0.0f;	//If a different character is being pressed, reset the timer
		}

		//Insert the current character at the end of the string, then set it to the last character pressed
		inputMessage.insert(inputMessage.getSize(), sf::String(inputManager->getCurrentChar()));
		last = inputManager->getCurrentChar();
	}

	else
	{
		last = 0;	//currentChar is 0 or less, no valid keys are being pressed, set last to reflect this
	}

	inputText->setString(inputMessage);	//Update the rendered text object with the new string
}

void ChatManager::addNewMessage(sf::String& name, sf::String& msg)
{
	inputMessage = sf::String("");	//Client has sent new message, clear the input buffer

	sf::Text message;

	message.setString(sf::String("[" + name + "]: " + msg));	//Format the client's username
	message.setFont(arial);
	message.setCharacterSize(24);
	message.setFillColor(sf::Color(224, 224, 224, 255));
	message.setPosition(sf::Vector2f(inputText->getPosition().x, inputText->getPosition().y - 30.0f));	//Display new message above the input box

	if (chatMessages.size() >= MAX_MESSAGES)	//Only a certain number of text messages should be displayed on the screen at once
	{
		chatMessages.erase(chatMessages.begin());	//Erase the oldest message
	}

	chatMessages.push_back(message);

	//Shift all message positions up to make room for the new one
	for (sf::Text& chat : chatMessages)
	{
		chat.setPosition(sf::Vector2f(chat.getPosition().x, chat.getPosition().y - 30.0f));
	}
}