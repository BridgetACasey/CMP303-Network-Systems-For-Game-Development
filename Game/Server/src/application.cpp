//@BridgetACasey

#include <iostream>

#include "application.h"

const int MAX_CLIENTS = 2;

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4444;

Application::Application()
{
	serverUDP = new sf::UdpSocket();
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

	if (serverUDP->bind(serverPortUDP, serverAddress) != sf::Socket::Done)
	{
		std::cout << "Failed to bind server to port " << serverPortUDP << std::endl;
	}
	
	else
	{
		serverUDP->setBlocking(false);
	}

	while (running)
	{
		if (selector.wait(sf::microseconds(1)))
		{
			if (selector.isReady(listener))
			{	//Check if listener is ready to receive new connections
				connectClients();
			}

			//Listener is not ready, check client sockets for new packets
			handleDataTCP();
		}

		handleDataUDP();

		disconnectClients();
	}
}

void Application::connectClients()
{
	if (clients.size() < MAX_CLIENTS)
	{
		sf::TcpSocket* clientTCP = new sf::TcpSocket();

		if (listener.accept(*clientTCP) == sf::Socket::Done)
		{
			sf::Packet packet;

			if (clientTCP->receive(packet) == sf::Socket::Done)
			{
				unsigned short udpPort;

				if (packet >> udpPort)
				{
					//Create TCP sockets for new connections and add them to selector
					Connection* client = new Connection(clients.size(), clientTCP);

					std::cout << "New client has connected on " << clientTCP->getRemoteAddress().toString() << std::endl;
					client->setClientUDP(udpPort);
					std::cout << "Client bound to port " << udpPort << std::endl;

					std::string spritePath = "assets/potatolizard.png";

					client->setSpritePath(spritePath);
					clients.push_back(client);

					selector.add(*clientTCP);
				}
			}
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
	for (int i = 0; i < clients.size(); i++)
	{
		if (!clients.at(i)->getConnected())
		{
			clients.erase(clients.begin() + i);
		}
	}

	clients.shrink_to_fit();

	for (int i = 0; i < clients.size(); i++)
	{
		clients.at(i)->setClientID(i);
	}
}

void Application::handleDataTCP()
{
	std::vector<ChatData> currentMessages;

	if (selector.wait(sf::microseconds(1)))
	{
		//Receiving new messages from the clients
		for (Connection* client : clients)
		{
			if (selector.isReady(*client->getClientTCP()))
			{
				sf::Packet receivedPacket;

				if (client->getClientTCP()->receive(receivedPacket) == sf::Socket::Done)
				{
					std::cout << "(TCP) RECEIVED packet successfully" << std::endl;

					ChatData chatData;
					bool quit;

					if (receivedPacket >> quit)
					{
						std::cout << "(TCP) CLIENT WISHES TO QUIT!" << std::endl;
						client->setConnected(false);
					}

					else if (receivedPacket >> chatData.userName >> chatData.messageBuffer)
					{
						std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;

						if (chatHistory.size() > 100)	//Only keeping a log of the most recent 100 messages sent to the server
						{
							chatHistory.erase(chatHistory.begin());
						}

						chatHistory.push_back(chatData);
						currentMessages.push_back(chatData);
					}
				}
			}
		}
	}

	//Sending messages back to clients
	for (Connection* client : clients)
	{
		for (auto& currentMsg : currentMessages)
		{
			sf::Packet sentPacket;

			if (sentPacket << currentMsg.userName << currentMsg.messageBuffer)
			{
				std::cout << "(TCP) PACKED data successfully" << std::endl;

				if (client->getClientTCP()->send(sentPacket) == sf::Socket::Done)
				{
					std::cout << "(TCP) SENT packet successfully" << std::endl;
				}
			}
		}
	}
}

void Application::handleDataUDP()
{
	//Receiving new messages from the clients
	sf::Packet receivedPacket;
	sf::IpAddress address;
	unsigned short port;

	if (serverUDP->receive(receivedPacket, address, port) == sf::Socket::Done)
	{
		//std::cout << "(UDP) RECEIVED packet from " << address.toString() << " on port " << port << std::endl;

		PlayerData playerData;

		if (receivedPacket >> playerData.id >> playerData.total >> playerData.posX >> playerData.posY >> playerData.velX >> playerData.velY >> playerData.spritePath)
		{
			//std::cout << "(UDP) UNPACKED data successfully - id: " << playerData.id << " pos x: " << playerData.posX << " pos y: " << playerData.posY << " vel x: " << playerData.velX << " vel y: " << playerData.velY << std::endl;
			for (Connection* client : clients)
			{
				playerData.total = clients.size();
				playerData.spritePath = client->getSpritePath();

				sf::Packet sentPacket;

				if (sentPacket << playerData.id << playerData.total << playerData.posX << playerData.posY << playerData.velX << playerData.velY << playerData.spritePath)
				{
					//std::cout << "(UDP) PACKED data successfully" << std::endl;

					//Sending messages back to clients
					if (serverUDP->send(sentPacket, address, client->getClientUDP()) == sf::Socket::Done)
					{
						std::cout << "(UDP) SENT packet successfully to port " << client->getClientUDP() << std::endl;
					}
				}
			}
		}
	}
}