//@BridgetACasey

#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "application.h"

const int MAX_CLIENTS = 2;

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4444;

Application::Application()
{
	//windowManager = new WindowManager();
	windowManager = nullptr;
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

		//sf::Event event;
		//
		//while (windowManager->getWindow()->pollEvent(event))
		//{
		//	if (event.type == sf::Event::Closed)
		//	{
		//		windowManager->getWindow()->close();
		//		running = false;
		//	}
		//
		//	update(getDeltaTime());
		//	render();
		//}
	}
}

void Application::update(float deltaTime)
{

}

void Application::render()
{
	windowManager->beginRender();

	//Render players and chat log

	windowManager->endRender();
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

			GameObject* player = new GameObject();

			player->setPosition(sf::Vector2f(575.0f, 300.0f));
			player->setSize(sf::Vector2f(50.0f, 50.0f));

			sf::Texture* playerTexture = new sf::Texture();

			if (!playerTexture->loadFromFile("assets/potatolizard.png"))
			{
				printf("could not load texture");
			}

			player->setTexture(playerTexture);

			clientPlayers.push_back(player);
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
					std::cout << "(TCP) RECEIVED packet successfully" << std::endl;

					ChatData chatData;

					if (receivedPacket >> chatData.userName >> chatData.messageBuffer)
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
	for (sf::TcpSocket* client : clientsTCP)
	{
		for (auto& currentMsg : currentMessages)
		{
			sf::Packet sentPacket;

			if (sentPacket << currentMsg.userName << currentMsg.messageBuffer)
			{
				std::cout << "(TCP) PACKED data successfully" << std::endl;

				if (client->send(sentPacket) == sf::Socket::Done)
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

		if (receivedPacket >> playerData.id >> playerData.total >> playerData.posX >> playerData.posY >> playerData.spritePath)
		{
			//std::cout << "(UDP) UNPACKED data successfully - id: " << playerData.id << " pos x: " << playerData.posX << " pos y: " << playerData.posY << std::endl;

			clientsUDP.insert(port);

			playerData.total = clientsUDP.size();

			sf::Packet sentPacket;

			if (sentPacket << playerData.id << playerData.total << playerData.posX << playerData.posY << playerData.spritePath)
			{
				//std::cout << "(UDP) PACKED data successfully" << std::endl;
			}

			//Sending messages back to clients
			for (auto& client : clientsUDP)
			{
				if (serverUDP->send(sentPacket, serverAddress, client) == sf::Socket::Done)
				{
					//std::cout << "(UDP) SENT packet successfully to port " << client << std::endl;
				}
			}
		}
	}
}