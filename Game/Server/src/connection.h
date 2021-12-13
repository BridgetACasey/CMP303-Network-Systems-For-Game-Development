//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>
#include <player_data.h>

//Represents and stores relevant data for a connected client
class Connection
{
public:
	Connection(int id, sf::TcpSocket* tcp);
	~Connection();

	bool validatePositions(PlayerData& data);
	bool insertPacket(PlayerData& data);
	void predictMovement();

	std::vector<PlayerData>& getPlayerPackets() { return playerPackets; }

	inline void setClientID(int id) { clientID = id; }
	inline int getClientID() const { return clientID; }

	inline sf::TcpSocket* getClientTCP() const { return clientTCP; }

	inline void setClientUDP(unsigned short udp) { clientUDP = udp; }
	inline sf::Uint16 getClientUDP() const { return clientUDP; }

	inline void setClientAddress(sf::Uint32 address) { clientAddress = sf::IpAddress(address); }
	inline sf::IpAddress& getClientAddress() { return clientAddress; }

private:
	std::vector<PlayerData> playerPackets;
	int clientID;	//Value ranging from 0 to 3 - the index of the client
	sf::TcpSocket* clientTCP;
	sf::TcpSocket* connectionTCP;
	sf::Uint16 clientUDP;	//The UDP port that the client has bound themselves to
	sf::IpAddress clientAddress;
};