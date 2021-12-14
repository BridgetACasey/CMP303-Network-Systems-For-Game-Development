//@BridgetACasey

#pragma once

#include <map>
#include <vector>

#include <SFML/Network.hpp>

#include "connection.h"
#include "player_data.h"
#include "chat_data.h"

//Contains all of the network functionality for the server application
class Application
{
public:
	Application();
	~Application();

	void run();

private:	
	void connectClients();
	void disconnectClients(sf::Packet& receivedPacket, unsigned int id);
	void eraseClients(int clientID, int clientFlag, sf::Uint16 clientPort);
	void verifyClients(int clientFlag, Connection* client);
	void checkClientTimeouts();

	void handleDataTCP();
	void handleDataUDP();

	void updateChatLog(sf::Packet& receivedPacket, ChatData& chatData);
	void updatePlayerData(sf::Packet& receivedPacket, sf::IpAddress& address);

	bool validateData(ChatData& chatData);
	bool validateData(PlayerData& playerData);

	bool checkTimeout(float last, float period);

	sf::Clock serverClock;

	float elapsedTime;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<Connection*> clients;
	std::vector<Connection*> timeoutClients;
	sf::UdpSocket* serverUDP;

	std::vector<ChatData> chatHistory;
};