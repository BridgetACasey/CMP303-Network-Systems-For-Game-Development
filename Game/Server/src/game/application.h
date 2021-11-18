//@BridgetACasey

#pragma once

#include <vector>
#include <SFML/Network.hpp>
#include "network/player_data.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void listenForClients();
	void receivePlayerData();

	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;
	sf::UdpSocket socket;
};