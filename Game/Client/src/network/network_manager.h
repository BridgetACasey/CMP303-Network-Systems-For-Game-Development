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

	bool requestConnection(const sf::String& address);

	bool sendDataTCP(ChatData& chatData, int quitFlag);
	bool receiveDataTCP(ChatData& chatData, int& quitFlag, sf::Uint16& playerPort);

	bool sendDataUDP(PlayerData& playerData);
	bool receiveDataUDP(PlayerData& playerData);

	bool validateData(ChatData& chatData);
	bool validateData(PlayerData& playerData);

	void insertPlayerData(PlayerData& playerData);
	bool insertChatData(ChatData& chatData);

	inline sf::TcpSocket* getSocketTCP() const { return socketTCP; }
	inline sf::UdpSocket* getSocketUDP() const { return socketUDP; }

private:
	sf::IpAddress serverAddress;
	
	//The client's TCP and UDP sockets
	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;

	//Pending data, used if partial packets have been received
	ChatData pendingChatData;
	PlayerData pendingPlayerData;

	std::vector<ChatData> previousChatData;
	std::vector<PlayerData> previousPlayerData;
};