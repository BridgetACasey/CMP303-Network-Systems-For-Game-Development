//@BridgetACasey

#include "connection.h"

struct reorder
{
	inline bool operator()(const PlayerData& d1, const PlayerData& d2)
	{
		return (d1.time < d2.time);
	}
};

Connection::Connection(int id, sf::TcpSocket* tcp)
{
	clientID = id;
	clientTCP = tcp;
	clientUDP = 0;
}

Connection::~Connection()
{
	if (clientTCP)
	{
		delete clientTCP;
	}
}

void Connection::insertPacket(PlayerData& data)
{
	if (playerPackets.size() > 1)
	{
		playerPackets.erase(playerPackets.begin());
	}

	playerPackets.push_back(data);

	std::sort(playerPackets.begin(), playerPackets.end(), reorder());
}

void Connection::predictMovement()
{
	if (playerPackets.size() < 2)
	{
		return;
	}

	float t = playerPackets.at(1).time - playerPackets.at(0).time;
	t /= 1000.0f;

	float dX = playerPackets.at(1).posX - playerPackets.at(0).posX;
	float dY = playerPackets.at(1).posY - playerPackets.at(0).posY;

	float vX = dX / t;
	float vY = dY / t;

	playerPackets.at(1).velX = vX;
	playerPackets.at(1).velY = vY;

	playerPackets.at(1).nextPosX = playerPackets.at(1).posX + dX;
	playerPackets.at(1).nextPosY = playerPackets.at(1).posY + dY;
}