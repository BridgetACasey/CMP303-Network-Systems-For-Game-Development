//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>
#include <player_data.h>

class Connection
{
public:
	Connection(int id, sf::TcpSocket* tcp);
	~Connection();

	void insertPacket(PlayerData& data);
	void predictMovement();

	std::vector<PlayerData>& getPlayerPackets() { return playerPackets; }

	inline void setClientID(int id) { clientID = id; }
	inline int getClientID() const { return clientID; }

	inline void setElapsedTime(float time) { lastTime = time; }
	inline float getLastTime() const { return lastTime; }

	inline void setConnected(bool con) { connected = con; }
	inline bool getConnected() const { return connected; }

	inline sf::TcpSocket* getClientTCP() const { return clientTCP; }

	inline void setClientUDP(unsigned short udp) { clientUDP = udp; }
	inline unsigned short getClientUDP() const { return clientUDP; }

private:
	std::vector<PlayerData> playerPackets;
	int clientID;
	float lastTime;
	bool connected;
	sf::TcpSocket* clientTCP;
	unsigned short clientUDP;
};