//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>

#include "network/player_data.h"
#include "network/chat_data.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void requestConnection();
	void requestDisconnection();

	bool sendDataTCP(ChatData& chatData);
	bool receiveDataTCP(ChatData& chatData);

	bool sendDataUDP(PlayerData& playerData);
	bool receiveDataUDP(PlayerData& playerData);

	bool validateData(ChatData& chatData);
	bool validateData(PlayerData& playerData);

	inline sf::TcpSocket* getSocketTCP() const { return socketTCP; }
	inline sf::UdpSocket* getSocketUDP() const { return socketUDP; }

private:
	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;

	ChatData pendingChatData;
	PlayerData pendingPlayerData;
};