//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>

#include "network/player_data.h"
#include "network/chat_data.h"

//Handles the bulk of network communication on the client side
class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void bindUDP();

	bool requestConnection();
	void requestDisconnection();

	bool sendDataTCP(ChatData& chatData);
	bool receiveDataTCP(ChatData& chatData);
	bool receiveChatData(ChatData& chatData, sf::Packet& receivedPacket);

	bool sendDataUDP(PlayerData& playerData);
	bool receiveDataUDP(PlayerData& playerData);

	bool validateData(ChatData& chatData);
	bool validateData(PlayerData& playerData);

	inline sf::TcpSocket* getSocketTCP() const { return socketTCP; }
	inline sf::UdpSocket* getSocketUDP() const { return socketUDP; }

private:
	//The client's TCP and UDP sockets
	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;

	//Pending data, used if partial packets have been received
	ChatData pendingChatData;
	PlayerData pendingPlayerData;
};