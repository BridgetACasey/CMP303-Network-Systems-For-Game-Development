//@BridgetACasey

#include "connection.h"

Connection::Connection(int id, sf::TcpSocket* tcp)
{
	clientID = id;
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