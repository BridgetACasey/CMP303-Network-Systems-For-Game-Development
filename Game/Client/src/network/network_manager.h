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

	void sendDataTCP(ChatData& chatData);
	void receiveDataTCP(ChatData& chatData);

	void sendDataUDP(PlayerData& playerData);
	void receiveDataUDP(PlayerData& playerData);

	inline sf::TcpSocket* getSocketTCP() const { return socketTCP; }
	inline sf::UdpSocket* getSocketUDP() const { return socketUDP; }

private:
	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;
};