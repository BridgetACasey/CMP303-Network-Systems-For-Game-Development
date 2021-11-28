//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>

class Connection
{
public:
	Connection(int id, sf::TcpSocket* tcp);
	~Connection();

	inline void addPosition(sf::Vector2f& pos) { positions.push_back(pos); }

	inline void setClientID(int id) { clientID = id; }
	inline int getClientID() const { return clientID; }

	inline void setElapsedTime(float time) { elapsedTime = time; }
	inline float getElapsedTime() const { return elapsedTime; }

	inline void setConnected(bool con) { connected = con; }
	inline bool getConnected() const { return connected; }

	inline sf::TcpSocket* getClientTCP() const { return clientTCP; }

	inline void setClientUDP(unsigned short udp) { clientUDP = udp; }
	inline unsigned short getClientUDP() const { return clientUDP; }

private:
	std::vector<sf::Vector2f> positions;
	int clientID;
	float elapsedTime;
	bool connected;
	sf::TcpSocket* clientTCP;
	unsigned short clientUDP;
};