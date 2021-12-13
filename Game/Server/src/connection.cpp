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

bool Connection::validatePositions(PlayerData& data)
{
	if (playerPackets.size() > 1)
	{
		float offset = 150.0f;

		//Position data received was technically valid but unrealistic, recalculate next position
		float lastDisX = abs(playerPackets.at(1).nextPosX - playerPackets.at(1).posX);
		float lastDisY = abs(playerPackets.at(1).nextPosY - playerPackets.at(1).posY);
		float newDisX = abs(data.nextPosX - data.posX);
		float newDisY = abs(data.nextPosY - data.posY);

		if (newDisX > (lastDisX + offset))
		{
			data.nextPosX = playerPackets.at(1).posX;
		}

		if (newDisY > (lastDisY + offset))
		{
			data.nextPosY = playerPackets.at(1).posY;
		}
	}

	return true;
}

bool Connection::insertPacket(PlayerData& data)
{
	if (validatePositions(data))
	{
		if (playerPackets.size() > 1)	//Checking vector size - only want to store two packets at a time
		{
			playerPackets.erase(playerPackets.begin());
		}

		playerPackets.push_back(data);

		//Reordering packets based on time received - mitigates problems related to receiving packets out of order
		std::sort(playerPackets.begin(), playerPackets.end(), reorder());

		return true;
	}

	return false;
}

void Connection::predictMovement()
{
	if (playerPackets.size() < 2)	//Wait until at least two packets have been received by the server to run prediction
	{
		return;
	}

	float t = playerPackets.at(1).time - playerPackets.at(0).time;
	t /= 1000.0f;	//Get difference in time as seconds, converting from milliseconds

	//Get displacement based on two positions
	float dX = playerPackets.at(1).posX - playerPackets.at(0).posX;
	float dY = playerPackets.at(1).posY - playerPackets.at(0).posY;

	//Calculate velocity based on displacement and time taken
	float vX = dX / t;
	float vY = dY / t;

	playerPackets.at(1).velX = vX;
	playerPackets.at(1).velY = vY;

	playerPackets.at(1).disX = dX;
	playerPackets.at(1).disY = dY;

	//Determine the predicted next position and send it back to clients
	playerPackets.at(1).nextPosX = playerPackets.at(1).posX + dX;
	playerPackets.at(1).nextPosY = playerPackets.at(1).posY + dY;
}