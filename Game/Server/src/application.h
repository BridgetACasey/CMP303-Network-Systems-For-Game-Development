//@BridgetACasey

#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include <SFML/Network.hpp>

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

	std::vector<sf::TcpSocket*> clientsTCP;
	std::unordered_set<unsigned short> clientsUDP;
	sf::UdpSocket* serverUDP;

	std::vector<ChatData> chatHistory;
};