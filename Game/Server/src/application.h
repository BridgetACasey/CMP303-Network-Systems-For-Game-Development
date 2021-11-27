//@BridgetACasey

#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include <SFML/Network.hpp>

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
	void connectClients();

	void handleDataTCP();
	void handleDataUDP();

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<Connection*> clients;
	sf::UdpSocket* serverUDP;

	std::vector<ChatData> chatHistory;
};