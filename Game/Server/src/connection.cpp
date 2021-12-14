//@BridgetACasey

#include "connection.h"

const int MIN_PACKETS = 2;
const int MAX_PACKETS = 5;

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
	lastUpdateTime = 0.0f;
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
		int last = playerPackets.size() - 1;

		float offset = 150.0f;

		//Position data received was technically valid but unrealistic, recalculate next position
		float lastDisX = abs(playerPackets.at(last).nextPosX - playerPackets.at(last).posX);
		float lastDisY = abs(playerPackets.at(last).nextPosY - playerPackets.at(last).posY);
		float newDisX = abs(data.nextPosX - data.posX);
		float newDisY = abs(data.nextPosY - data.posY);

		if (newDisX > (lastDisX + offset))
		{
			data.nextPosX = playerPackets.at(last).posX;
		}

		if (newDisY > (lastDisY + offset))
		{
			data.nextPosY = playerPackets.at(last).posY;
		}
	}

	return true;
}

bool Connection::insertPacket(PlayerData& data)
{
	if (validatePositions(data))
	{
		if (playerPackets.size() > (MAX_PACKETS - 1))	//Checking vector size - only want to store a certain amount of packed data
		{
			playerPackets.erase(playerPackets.begin());
		}

		playerPackets.push_back(data);

		//Reordering packet data based on time received - mitigates problems related to receiving packets out of order
		std::sort(playerPackets.begin(), playerPackets.end(), reorder());

		return true;
	}

	return false;
}

void Connection::predictMovement()
{
	if (playerPackets.size() < MIN_PACKETS)	//Wait until at least three packets have been received by the server to run prediction
	{
		return;
	}

	int first = playerPackets.size() - 1;	//Grab the index of the most recent packet
	int second = first - 1;

	float t = playerPackets.at(first).time - playerPackets.at(second).time;
	t /= 1000.0f;	//Get difference in time as seconds, converting from milliseconds

	//Get displacement based on two positions
	float dX = playerPackets.at(first).posX - playerPackets.at(second).posX;
	float dY = playerPackets.at(first).posY - playerPackets.at(second).posY;

	//Calculate velocity based on displacement and time taken
	float vX = dX / t;
	float vY = dY / t;

	playerPackets.at(first).velX = vX;
	playerPackets.at(first).velY = vY;

	playerPackets.at(first).disX = dX;
	playerPackets.at(first).disY = dY;

	//Determine the predicted next position and send it back to clients
	playerPackets.at(first).nextPosX = playerPackets.at(first).posX + dX;
	playerPackets.at(first).nextPosY = playerPackets.at(first).posY + dY;
}