//@BridgetACasey

#include <iostream>

#include "network_manager.h"

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4444;

NetworkManager::NetworkManager()
{
	socketTCP = new sf::TcpSocket();
	socketUDP = new sf::UdpSocket();

	if (socketUDP->bind(sf::Socket::AnyPort) != sf::UdpSocket::Done)
	{
		std::cout << "UDP socket failed to bind to port" << std::endl;
	}

	else
	{
		std::cout << "UDP socket bound to port " << socketUDP->getLocalPort() << std::endl;
		//socketUDP->setBlocking(false);
	}
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

		sf::Socket::Status status = socketTCP->send(packet);

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "(TCP) SENT packet successfully" << std::endl;
			break;

		case sf::Socket::Partial:
			std::cout << "(TCP) PARTIAL packet sent" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "(TCP) ERROR sending packet" << std::endl;
			break;

		default:
			break;
		}
	}
}

void NetworkManager::receiveDataTCP(ChatData& chatData)
{
	sf::Packet packet;

	sf::Socket::Status status = socketTCP->receive(packet);

	switch (status)
	{
	case sf::Socket::Done:
		std::cout << "(TCP) RECEIVED packet successfully" << std::endl;
		if (packet >> chatData.userName >> chatData.messageBuffer)
		{
			std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
		}
		break;

	case sf::Socket::NotReady:
		//std::cout << "(TCP) NOT READY to receive packet" << std::endl;
		break;

	case sf::Socket::Partial:
		std::cout << "(TCP) PARTIAL packet received" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "(TCP) ERROR receiving packet" << std::endl;
		break;

	default:
		break;
	}
}

void NetworkManager::sendDataUDP(PlayerData& playerData)
{
	sf::Packet packet;

	if (packet << playerData.id << playerData.total << playerData.posX << playerData.posY << playerData.velX << playerData.velY << playerData.spritePath)
	{
		//std::cout << "(UDP) PACKED data successfully" << std::endl;

		sf::Socket::Status status = socketUDP->send(packet, serverAddress, serverPortUDP);

		switch (status)
		{
		case sf::Socket::Done:
			//std::cout << "(UDP) SENT packet successfully" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "(UDP) ERROR sending packet" << std::endl;
			break;

		default:
			break;
		}
	}
}

void NetworkManager::receiveDataUDP(PlayerData& playerData)
{
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;

	sf::Socket::Status status = socketUDP->receive(packet, address, port);

	switch (status)
	{
	case sf::Socket::Done:
		//std::cout << "(UDP) SENT packet successfully" << std::endl;
		if (packet >> playerData.id >> playerData.total >> playerData.posX >> playerData.posY >> playerData.velX >> playerData.velY >> playerData.spritePath)
		{
			//std::cout << "(UDP) UNPACKED data successfully - id: " << playerData.id << " pos x: " << playerData.posX << " pos y: " << playerData.posY << " vel x: " << playerData.velX << " vel y: " << playerData.velY << std::endl;
		}
		break;

	case sf::Socket::NotReady:
		std::cout << "(UDP) NOT READY to receive packet" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "(UDP) ERROR receiving packet" << std::endl;
		break;

	default:
		break;
	}
}