//@BridgetACasey

#include <iostream>

#include "application.h"

const int MAX_CLIENTS = 4;

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

					int clientFlag = -1;
					int clientPort = 0;

					//Inform newly connected client of other clients on server
					for (Connection* c : clients)
					{
						sf::Packet updateNew;

						clientPort = (int)c->getClientUDP();

						if (updateNew << clientFlag << clientPort)
						{
							if (client->getClientTCP()->send(updateNew) == sf::Socket::Done)
							{
								std::cout << "(TCP) Informing client " << client->getClientUDP() << " of connection at " << clientPort << std::endl;
							}
						}
					}

					//Inform other clients on the server of the newly connected client
					sf::Packet updateOther;
					clientPort = (int)udpPort;

					if (updateOther << clientFlag << clientPort)
					{
						for (Connection* cn : clients)
						{
							if (cn->getClientTCP()->send(updateOther) == sf::Socket::Done)
							{
								std::cout << "(TCP) Informing client " << cn->getClientUDP() << " of connection at " << clientPort << std::endl;
							}
						}
					}
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
	int clientFlag = -2;
	int clientPort = clients.at(id)->getClientUDP();

	if (receivedPacket >> quit)
	{
		sf::Packet sentPacket;

		if (quit == -1)
		{
			std::cout << "(TCP) Client wishes to QUIT!" << std::endl;
			
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

	sf::Packet updatePacket;

	if (updatePacket << clientFlag << clientPort)
	{
		for (Connection* client : clients)
		{
			if (client->getClientTCP()->send(updatePacket) == sf::Socket::Done)
			{
				std::cout << "(TCP) Informing client " << client->getClientUDP() << " of disconnection at " << clientPort << std::endl;
			}
		}
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

	if (receivedPacket >> playerData.time >> playerData.id >> playerData.posX >> playerData.posY >> playerData.nextPosX >> playerData.nextPosY >> playerData.velX >> playerData.velY)
	{
		for (Connection* client : clients)
		{
			if (client->getClientUDP() == playerData.id)
			{
				for (auto& data : client->getPlayerPackets())	//Avoiding duplicate packets
				{
					if (data.time == playerData.time)
					{
						return;
					}
				}

				client->insertPacket(playerData);
				client->predictMovement();
				playerData = client->getPlayerPackets().back();
			}
		}
	}

	sf::Packet sentPacket;

	for (Connection* client : clients)
	{
		if (client->getClientUDP() == playerData.id)
		{
			continue;
		}

		if (sentPacket << playerData.time << playerData.id << playerData.posX << playerData.posY << playerData.nextPosX << playerData.nextPosY << playerData.velX << playerData.velY)
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