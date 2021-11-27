//@BridgetACasey

#pragma once

#include <SFML/Network.hpp>

class Connection
{
public:
	Connection(int id, sf::TcpSocket* tcp);
	~Connection();

	inline void setClientID(int id) { clientID = id; }
	inline int getClientID() const { return clientID; }

	inline void setConnected(bool con) { connected = con; }
	inline bool getConnected() const { return connected; }

	inline void setSpritePath(std::string& sprite) { spritePath = sprite; }
	inline std::string getSpritePath() const { return spritePath; }

	inline sf::TcpSocket* getClientTCP() const { return clientTCP; }

	inline void setClientUDP(unsigned short udp) { clientUDP = udp; }
	inline unsigned short getClientUDP() const { return clientUDP; }

private:
	int clientID;
	bool connected;
	sf::TcpSocket* clientTCP;
	unsigned short clientUDP;
	std::string spritePath;
};