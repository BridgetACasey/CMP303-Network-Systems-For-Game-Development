//@BridgetACasey

#include <iostream>

#include "application.h"

const int MAX_CLIENTS = 1;

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4444;

Application::Application()
{

}

Application::~Application()
{

}

void Application::run()
{
	bool running = true;
	
	std::cout << "Waiting for client..." << std::endl;

	listener.listen(serverPortTCP);

	selector.add(listener);

	while (running)
	{
		if (selector.wait(sf::microseconds(1)))
		{
			if (selector.isReady(listener))
			{	//Check if listener is ready to receive new connections, or if clients want to disconnect
				connectClients();
				disconnectClients();
			}

			//Listener is not ready, check client sockets for new packets
			handleDataTCP();
			handleDataUDP();
		}
	}
}

void Application::connectClients()
{
	if (clientsTCP.size() < MAX_CLIENTS)
	{
		sf::TcpSocket* clientTCP = new sf::TcpSocket();

		if (listener.accept(*clientTCP) == sf::Socket::Done)
		{
			//Create TCP sockets for new connections and add them to selector
			clientsTCP.push_back(clientTCP);

			selector.add(*clientTCP);

			std::cout << "New client has connected on " << clientTCP->getRemoteAddress().toString() << std::endl;

			//Create UDP sockets for new connections
			sf::UdpSocket* clientUDP = new sf::UdpSocket();

			clientUDP->bind(serverPortUDP);

			clientsUDP.push_back(clientUDP);
		}

		else
		{
			std::cout << "Error connecting client" << std::endl;

			delete clientTCP;	//Client cannot connect, delete TCP socket
		}
	}
}

void Application::disconnectClients()
{
	//disconnect client and resize vectors
}

void Application::handleDataTCP()
{
	std::vector<ChatData> currentMessages;

	if (selector.wait(sf::microseconds(1)))
	{
		//Receiving new messages from the clients
		for (sf::TcpSocket* client : clientsTCP)
		{
			if (selector.isReady(*client))
			{
				sf::Packet receivedPacket;

				if (client->receive(receivedPacket) == sf::Socket::Done)
				{
					std::cout << "RECEIVED packet successfully" << std::endl;

					ChatData chatData;

					if (receivedPacket >> chatData.userName >> chatData.messageBuffer)
					{
						std::cout << "UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;

						chatHistory.push_back(chatData);
						currentMessages.push_back(chatData);
					}
				}
			}
		}
	}

	//Sending messages back to clients
	for (sf::TcpSocket* client : clientsTCP)
	{
		for (auto& currentMsg : currentMessages)
		{
			sf::Packet sentPacket;

			if (sentPacket << currentMsg.userName << currentMsg.messageBuffer)
			{
				std::cout << "PACKED data successfully" << std::endl;

				if (client->send(sentPacket) == sf::Socket::Done)
				{
					std::cout << "SENT packet successfully" << std::endl;
				}
			}
		}
	}
}

void Application::handleDataUDP()
{
	//receive packets from clients through UDP
}