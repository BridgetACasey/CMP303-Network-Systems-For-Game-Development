//@BridgetACasey

#include <iostream>

#include "application.h"

//Operator overloads for reading and writing between packets and frequently used struct types
sf::Packet operator << (sf::Packet& packet, const PlayerData& data)
{
	return packet << data.time << data.port << data.posX << data.posY << data.nextPosX << data.nextPosY << data.velX << data.velY << data.disX << data.disY;
}

sf::Packet operator >> (sf::Packet& packet, PlayerData& data)
{
	return packet >> data.time >> data.port >> data.posX >> data.posY >> data.nextPosX >> data.nextPosY >> data.velX >> data.velY >> data.disX >> data.disY;
}

sf::Packet operator << (sf::Packet& packet, const ChatData& data)
{
	return packet << data.time << data.userName << data.messageBuffer;
}

sf::Packet operator >> (sf::Packet& packet, ChatData& data)
{
	return packet >> data.time >> data.userName >> data.messageBuffer;
}

const int MAX_CLIENTS = 4;

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();	//Server address by default is 192.168.0.18 on client side, this information must be given to client outside of the application
const sf::Uint16 serverPortTCP = 5555;
const sf::Uint16 serverPortUDP = 4444;

Application::Application()
{
	serverUDP = new sf::UdpSocket();
}

Application::~Application()
{

}

//Launch and run the server application
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

	//Infinite server loop
	while (running)
	{
		elapsedTime = serverClock.getElapsedTime().asMilliseconds();

		//std::cout << "Time: " << elapsedTime << std::endl;

		if (selector.wait(sf::microseconds(1)))
		{
			if (selector.isReady(listener))
			{	//Check if listener is ready to receive new connections
				connectClients();
			}

			disconnectClients();

			//Listener is not ready, check client sockets for new packets
			handleDataTCP();
		}

		handleDataUDP();
	}
}

//Connect clients through TCP so long as there are still less than 4 logged on to the server
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
				sf::Uint16 newClientPort;	//Receive the UDP port that the client is bound to
				sf::Uint32 newClientAddress;

				if (packet >> newClientPort >> newClientAddress)
				{
					//Create TCP sockets for new connections and add them to selector
					Connection* client = new Connection(clients.size(), clientTCP);

					client->setClientUDP(newClientPort);
					client->setClientAddress(newClientAddress);

					std::cout << "New client has connected on " << client->getClientAddress().toString() << std::endl;
					std::cout << "Client bound to port " << client->getClientUDP() << std::endl;

					client->setLastUpdateTime(elapsedTime);

					clients.push_back(client);

					selector.add(*clientTCP);

					int clientFlag = -3;	//Send a flag to the clients to determine what to do next: -3 for creating new player instances
					sf::Uint16 clientPort = 0;	//Let the current client know what ports the others are bound to - this is then used as a type of id

					//Inform newly connected client of other clients on server
					for (Connection* c : clients)
					{
						sf::Packet updateNew;

						clientPort = c->getClientUDP();

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
					clientPort = newClientPort;

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

void Application::disconnectClients()
{
	for (Connection* client : clients)
	{
		if (checkTimeout(client->getLastUpdateTime(), 15000.0f))
		{
			timeoutClients.push_back(client);
		}
	}

	if (timeoutClients.size() > 0)
	{
		for (Connection* timeout : timeoutClients)
		{
			std::cout << "Client " << std::to_string(timeout->getClientUDP()) << " timed out!" << std::endl;
			eraseClients(timeout->getClientID(), -4, timeout->getClientUDP());
		}
	}
}

//Disconnect clients gracefully and erase their data after receiving the appropriate flag
void Application::disconnectClients(sf::Packet& receivedPacket, unsigned int id)
{
	int quit;
	int clientFlag = -4;	//Flag value of -4 means a player instance needs to be removed, as the client has disconnected
	sf::Uint16 clientPort = clients.at(id)->getClientUDP();

	if (receivedPacket >> quit)
	{
		sf::Packet sentPacket;

		if (quit == -1)
		{
			std::cout << "(TCP) Client wishes to QUIT!" << std::endl;
			
			//Confirming that request to disconnect has been received
			quit = -2;

			if (sentPacket << quit)
			{
				//Inform client that their request has been accepted and erase them from the list of connected clients
				if (clients.at(id)->getClientTCP()->send(sentPacket) == sf::Socket::Done)
				{
					eraseClients(id, clientFlag, clientPort);
				}
			}
		}
	}
}

void Application::eraseClients(int clientID, int clientFlag, sf::Uint16 clientPort)
{
	std::cout << "(TCP) Disconnecting client..." << std::endl;

	selector.remove(*clients.at(clientID)->getClientTCP());
	clients.erase(clients.begin() + clientID);

	clients.shrink_to_fit();

	//Reordering client IDs after the disconnected one has been removed
	for (int i = 0; i < clients.size(); i++)
	{
		clients.at(i)->setClientID(i);
	}

	sf::Packet updatePacket;

	//Let other clients know about the disconnection so they can delete the respective player object
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

	timeoutClients.erase(timeoutClients.begin(), timeoutClients.end());
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

					if (receivedPacket.getDataSize() == sizeof(int))	//If only a single int has been received, client wishes to disconnect
					{
						disconnectClients(receivedPacket, i);
					}

					else
					{
						ChatData chatData;	//Otherwise, the packet most likely contains a chat message
						chatData.userName = "NULL";
						updateChatLog(receivedPacket, chatData);

						if (chatData.userName != "NULL")
						{
							currentMessages.push_back(chatData);
							clients.at(i)->setLastUpdateTime(elapsedTime);
						}
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

				if (sentPacket << currentMsg)
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
	if (receivedPacket >> chatData)
	{
		if (!validateData(chatData))
		{
			return;
		}

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

	if (receivedPacket >> playerData)
	{
		if (!validateData(playerData))
		{
			return;
		}

		for (Connection* client : clients)
		{
			if (client->getClientUDP() == playerData.port)
			{
				for (auto& data : client->getPlayerPackets())	//Avoiding duplicate packets
				{
					if (data.time == playerData.time)
					{
						return;
					}
				}

				if (client->insertPacket(playerData))
				{
					client->predictMovement();
				}

				client->setLastUpdateTime(elapsedTime);

				playerData = client->getPlayerPackets().back();	//Grab the last packet after prediction has been calculated
			}
		}
	}

	sf::Packet sentPacket;

	//Sending data for the current client back to all clients logged on
	for (Connection* client : clients)
	{
		if (sentPacket << playerData)
		{
			if (serverUDP->send(sentPacket, client->getClientAddress(), client->getClientUDP()) == sf::Socket::Done)
			{
				//std::cout << "(UDP) SENT packet successfully to port " << client->getClientUDP() << std::endl;
			}
		}
	}
}

//Checking that received packets contain sensible data, otherwise they have likely been tampered with
bool Application::validateData(ChatData& chatData)
{
	if (chatData.userName.getSize() > 16)
		return false;
	if (chatData.messageBuffer.getSize() > 48)
		return false;

	for (char n : chatData.userName)
	{
		if (n < 31 || n > 127)
		{
			return false;
		}
	}

	for (char msg : chatData.messageBuffer)
	{
		if (msg < 31 || msg > 127)
		{
			return false;
		}
	}

	int matchingNames = 0;

	for (Connection* client : clients)
	{
		sf::String name = "P_" + std::to_string(client->getClientUDP());

		if (name == chatData.userName)
		{
			matchingNames++;
		}
	}

	if (matchingNames == 0)
	{
		return false;
	}

	return true;
}

//Checking that received packets contain sensible data, otherwise they have likely been tampered with
bool Application::validateData(PlayerData& playerData)
{
	if (playerData.port < 0 || playerData.port > 65535)
		return false;
	if (playerData.time < 0.0f)
		return false;
	if (playerData.posX < -5.0f || playerData.posX > 1200.0f)
		return false;
	if (playerData.posY < -5.0f || playerData.posY > 750.0f)
		return false;
	if (playerData.nextPosX < -5.0f || playerData.nextPosX > 1200.0f)
		return false;
	if (playerData.nextPosY < -5.0f || playerData.nextPosY > 750.0f)
		return false;
	if (playerData.velX < -300.0f || playerData.velX > 300.0f)
		return false;
	if (playerData.velY < -300.0f || playerData.velY > 300.0f)
		return false;

	return true;
}

bool Application::checkTimeout(float last, float period)
{
	if ((elapsedTime - last) >= period)
	{
		return true;
	}

	return false;
}