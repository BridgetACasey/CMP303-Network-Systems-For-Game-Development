//@BridgetACasey

#include <iostream>

#include "network_manager.h"

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4444;
const int clientPortUDP = 4445;

NetworkManager::NetworkManager()
{
	socketTCP = new sf::TcpSocket();
	socketUDP = new sf::UdpSocket();

	socketUDP->bind(clientPortUDP);
	socketUDP->setBlocking(false);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::requestConnection()
{
	if(socketTCP->connect(serverAddress, serverPortTCP) != sf::TcpSocket::Done)
	{
		std::cout << "Failed to connect to server" << std::endl;
	}

	socketTCP->setBlocking(false);
}

void NetworkManager::requestDisconnection()
{
	socketTCP->disconnect();
}

void NetworkManager::sendDataTCP(ChatData& chatData)
{
	sf::Packet packet;

	if (packet << chatData.userName << chatData.messageBuffer)
	{
		std::cout << "(TCP) PACKED data successfully" << std::endl;

		if (socketTCP->send(packet) == sf::Socket::Done)
		{
			std::cout << "(TCP) SENT packet successfully" << std::endl;
		}
	}
}

void NetworkManager::receiveDataTCP(ChatData& chatData)
{
	sf::Packet packet;

	if (socketTCP->receive(packet) == sf::Socket::Done)
	{
		std::cout << "(TCP) RECEIVED packet successfully" << std::endl;

		if (packet >> chatData.userName >> chatData.messageBuffer)
		{
			std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
		}
	}
}

void NetworkManager::sendDataUDP(PlayerData& playerData)
{
	sf::Packet packet;

	if (packet << playerData.id << playerData.posX << playerData.posY << playerData.spritePath)
	{
		//std::cout << "(UDP) PACKED data successfully" << std::endl;

		if (socketUDP->send(packet, serverAddress, serverPortUDP) == sf::Socket::Done)
		{
			//std::cout << "(UDP) SENT packet successfully" << std::endl;
		}
	}
}

void NetworkManager::receiveDataUDP(PlayerData& playerData)
{
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;

	if (socketUDP->receive(packet, address, port) == sf::Socket::Done)
	{
		std::cout << "(UDP) RECEIVED packet from " << address.toString() << " on port " << port << std::endl;

		if (packet >> playerData.id >> playerData.posX >> playerData.posY >> playerData.spritePath)
		{
			std::cout << "(UDP) UNPACKED data successfully - id: " << playerData.id << " pos x: " << playerData.posX << " pos y: " << playerData.posY << std::endl;
		}
	}
}