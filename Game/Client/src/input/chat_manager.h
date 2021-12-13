//@BridgetACasey

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "input_manager.h"

//Handles the input stream for the chat feature
class ChatManager
{
public:
	ChatManager(InputManager* input);
	ChatManager(InputManager* input, sf::Text* inputT, const int maxInput);
	~ChatManager();

	void updateMessageStream(float deltaTime);
	void addNewMessage(sf::String& name, sf::String& msg);

	inline void setInputMessage(const sf::String& message) { inputMessage = message; }
	inline sf::Text* getInputText() const { return inputText; }
	const inline std::vector<sf::Text>& getChatMessages() const { return chatMessages; }

private:
	InputManager* inputManager;
	
	sf::Font arial;
	sf::Text* inputText;	//The rendered text object storing the contents of inputMessage
	sf::String inputMessage;	//The string that is written to when the user presses a printable key

	std::vector<sf::Text> chatMessages;	//The log of chat messages sent and received by the client

	char last;	//The last character that was inserted into the message stream
	float delay;	//Timer variable used when client wants to insert a series of duplicate characters

	int MAX_MESSAGES;
	int MAX_INPUT;
};