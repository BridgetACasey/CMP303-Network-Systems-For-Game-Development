//@BridgetACasey

#include "application.h"

const int MAX_CLIENTS = 1;

Application::Application()
{
	serverAddress = sf::IpAddress::getLocalAddress();
	serverPort = 5555;
}

Application::~Application()
{

}

void Application::run()
{
	bool running = true;
	
	printf("Waiting for client...");

	listener.listen(serverPort);

	selector.add(listener);

	while (running)
	{
		if (clientsTCP.size() < MAX_CLIENTS)
		{
			if (selector.wait(sf::microseconds(1)));
			{
				if (selector.isReady(listener))
				{
					connectClients();
					//disconnectClients();
				}

				else
				{
					receiveData();
				}
			}
		}
	}
}

void Application::connectClients()
{
	sf::TcpSocket* clientTCP = new sf::TcpSocket();

	if (listener.accept(*clientTCP) == sf::Socket::Done)
	{
		//Create TCP sockets for new connections and add them to selector
		clientsTCP.push_back(clientTCP);

		selector.add(*clientTCP);

		printf("Client has connected!");

		//Create UDP sockets for new connections
		sf::UdpSocket* clientUDP = new sf::UdpSocket();

		clientsUDP.push_back(clientUDP);
	}

	else
	{
		printf("Error connecting client");

		delete clientTCP;	//Client cannot connect, delete TCP socket
	}
}

void Application::disconnectClients()
{
	//disconnect client and resize vectors
}

void Application::receiveData()
{
	for (sf::TcpSocket* client : clientsTCP)
	{
		if (selector.isReady(*client))
		{
			sf::Packet packet;

			if (client->receive(packet) == sf::Socket::Done)
			{

			}
		}
	}
}