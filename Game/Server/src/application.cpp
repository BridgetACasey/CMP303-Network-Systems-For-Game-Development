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

void Application::disconnectClients(sf::Packet& receivedPacket, int id)
{
	int quit;

	if (receivedPacket >> quit)
	{
		sf::Packet sentPacket;

		if (quit == -1)
		{
			std::cout << "(TCP) Client wishes to QUIT!" << std::endl;
			clients.at(id)->setConnected(false);
			quit = -2;

			if (sentPacket << quit)
			{
				if (clients.at(id)->getClientTCP()->send(sentPacket) == sf::Socket::Done)
				{
					std::cout << "(TCP) Disconnecting client..." << std::endl;

					selector.remove(*clients.at(id)->getClientTCP());
					clients.erase(clients.begin() + id);
				}
			}
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
		for (int i = 0; i < clients.size(); i++)
		{
			if (selector.isReady(*clients.at(i)->getClientTCP()))
			{
				sf::Packet receivedPacket;

				if (clients.at(i)->getClientTCP()->receive(receivedPacket) == sf::Socket::Done)
				{
					std::cout << "(TCP) RECEIVED packet successfully" << std::endl;

					if (receivedPacket.getDataSize() == sizeof(int))
					{
						std::cout << "TCP PACKET RECEIVED - CLIENT WANTS TO QUIT!" << std::endl;
						disconnectClients(receivedPacket, i);
					}

					else
					{
						ChatData chatData;
						std::cout << "TCP PACKET RECEIVED - CLIENT WANTS TO CHAT!" << std::endl;
						updateChatLog(receivedPacket, chatData);
						currentMessages.push_back(chatData);
					}
				}
			}
		}
	}

	//Sending messages back to clients
	if (currentMessages.size() > 0)
	{
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

		updatePlayerData(receivedPacket, address);
	}
}

void Application::updateChatLog(sf::Packet& receivedPacket, ChatData& chatData)
{
	if (receivedPacket >> chatData.userName >> chatData.messageBuffer)
	{
		std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
	
		if (chatHistory.size() > 100)	//Only keeping a log of the most recent 100 messages sent to the server
		{
			chatHistory.erase(chatHistory.begin());
		}
	
		chatHistory.push_back(chatData);
	}
}

void Application::updatePlayerData(sf::Packet& receivedPacket, sf::IpAddress& address)
{
	PlayerData playerData;

	if (receivedPacket >> playerData.time >> playerData.id >> playerData.total >> playerData.posX >> playerData.posY >> playerData.velX >> playerData.velY)
	{
		playerData.total = clients.size();

		sf::Packet sentPacket;

		for (Connection* client : clients)
		{
			if (client->getClientUDP() == playerData.id)
			{
				sf::Vector2f pos = sf::Vector2f(playerData.posX, playerData.posY);
				client->addPosition(pos);

				if (playerData.time > (client->getElapsedTime() + 100.0f))
				{
					predictMovement(playerData, client);
					client->setElapsedTime(playerData.time);
					std::cout << "Elapsed Time (" << client->getClientUDP() << "): " << playerData.time << std::endl;
				}
			}

			if (sentPacket << playerData.time << playerData.id << playerData.total << playerData.posX << playerData.posY << playerData.velX << playerData.velY)
			{
				//std::cout << "(UDP) PACKED data successfully" << std::endl;

				//Sending messages back to clients
				if (serverUDP->send(sentPacket, address, client->getClientUDP()) == sf::Socket::Done)
				{
					//std::cout << "(UDP) SENT packet successfully to port " << client->getClientUDP() << std::endl;
				}
			}
		}
	}
}

PlayerData& Application::predictMovement(PlayerData& player, Connection* client)
{
	float time = player.time - client->getElapsedTime();
	time /= 1000.0f;

	float displacementX = 0.5f * player.velX * time;
	float displacementY = 0.5f * player.velY * time;

	player.posX += displacementX;
	player.posY += displacementY;

	return player;
}