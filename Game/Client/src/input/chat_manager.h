//@BridgetACasey

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "input_manager.h"

class ChatManager
{
public:
	ChatManager(InputManager* input);
	~ChatManager();

	void updateMessageStream(float deltaTime);
	void addNewMessage(sf::String& name, sf::String& msg);

	inline sf::Text* getInputText() const { return inputText; }
	const inline std::vector<sf::Text>& getChatMessages() const { return chatMessages; }

private:
	InputManager* inputManager;
	
	sf::Font arial;
	sf::Text* inputText;
	sf::String inputMessage;

	std::vector<sf::Text> chatMessages;

	char last;
	float delay;
};