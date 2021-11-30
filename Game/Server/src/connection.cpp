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
	lastTime = 0.0f;
	connected = true;
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
	if (playerPackets.size() > 2)
	{
		playerPackets.erase(playerPackets.begin());
	}

	playerPackets.push_back(data);

	std::sort(playerPackets.begin(), playerPackets.end(), reorder());
}

void Connection::predictMovement()
{
/* • Equations of motion
	• next position = previous position + displacement
	• displacement = speed * time (since the last message)
	• speed = distance between last two positions / time between last two positions
*/

	if (playerPackets.size() < 3)
	{
		return;
	}

	float t = playerPackets.at(2).time - playerPackets.at(1).time;
	t /= 1000.0f;

	float dX = playerPackets.at(2).posX - playerPackets.at(1).posX;
	float dY = playerPackets.at(2).posY - playerPackets.at(1).posY;

	float vX = dX / t;
	float vY = dY / t;

	playerPackets.at(2).velX = vX;
	playerPackets.at(2).velY = vY;

	playerPackets.at(2).nextPosX = playerPackets.at(2).posX + dX;
	playerPackets.at(2).nextPosY = playerPackets.at(2).posY + dY;
}