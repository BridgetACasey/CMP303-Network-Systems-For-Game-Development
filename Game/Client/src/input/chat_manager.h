//@BridgetACasey

#pragma once

#include "input_manager.h"

class ChatManager
{
public:
	ChatManager(InputManager* input);
	~ChatManager();

	void updateMessageStream(float deltaTime);

	const inline sf::String& getMessage() { return message; }

private:
	InputManager* inputManager;
	
	sf::String message;

	char last;

	float delay;
};