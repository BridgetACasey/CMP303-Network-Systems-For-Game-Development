//@BridgetACasey

#pragma once

#include <map>
#include <vector>

#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>

#include "connection.h"
#include "player_data.h"
#include "chat_data.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	inline int getServerTime() const { return serverClock.getElapsedTime().asMilliseconds(); }
	
	void connectClients();
	void disconnectClients();

	void handleDataTCP();
	void handleDataUDP();

	sf::Clock serverClock;

	int elapsedTime;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<Connection*> clients;
	sf::UdpSocket* serverUDP;

	std::vector<ChatData> chatHistory;
};