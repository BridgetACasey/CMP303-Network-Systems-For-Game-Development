//@BridgetACasey

#pragma once

#include <vector>
#include <SFML/Network.hpp>
#include "player_data.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void connectClients();
	void disconnectClients();

	void receiveData();

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<sf::TcpSocket*> clientsTCP;
	std::vector<sf::UdpSocket*> clientsUDP;

	sf::IpAddress serverAddress;
	int serverPort;
};