//@BridgetACasey

#pragma once

#include <vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Network.hpp>
#include "window_manager.h"
#include "game_object.h"
#include "player_data.h"
#include "chat_data.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	inline float getDeltaTime() { return gameClock.restart().asSeconds(); }

private:
	void update(float deltaTime);
	void render();
	
	void connectClients();
	void disconnectClients();

	void handleDataTCP();
	void handleDataUDP();

	sf::Clock gameClock;
	std::vector<GameObject*> clientPlayers;
	
	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<sf::TcpSocket*> clientsTCP;
	std::vector<sf::UdpSocket*> clientsUDP;

	std::vector<ChatData> chatHistory;

	WindowManager* windowManager;
};