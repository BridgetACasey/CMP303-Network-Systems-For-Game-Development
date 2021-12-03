//@BridgetACasey

#pragma once

#include <map>
#include <vector>

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
	void disconnectClients(sf::Packet& receivedPacket, int id);

	void handleDataTCP();
	void handleDataUDP();

	bool validateData(ChatData& chatData);
	bool validateData(PlayerData& playerData);

	void updateChatLog(sf::Packet& receivedPacket, ChatData& chatData);
	void updatePlayerData(sf::Packet& receivedPacket, sf::IpAddress& address);

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<Connection*> clients;
	sf::UdpSocket* serverUDP;

	std::vector<ChatData> chatHistory;
};