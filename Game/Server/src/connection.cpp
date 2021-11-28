//@BridgetACasey

#include "connection.h"

Connection::Connection(int id, sf::TcpSocket* tcp)
{
	clientID = id;
	elapsedTime = 0.0f;
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