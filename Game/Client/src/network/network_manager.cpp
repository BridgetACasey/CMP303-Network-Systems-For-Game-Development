//@BridgetACasey

#include <iostream>

#include "network_manager.h"

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const int serverPortTCP = 5555;
const int serverPortUDP = 4445;

NetworkManager::NetworkManager()
{
	socketTCP = new sf::TcpSocket();
	socketUDP = new sf::UdpSocket();

	socketUDP->bind(serverPortUDP);
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
		std::cout << "PACKED data successfully" << std::endl;

		if (socketTCP->send(packet) == sf::Socket::Done)
		{
			std::cout << "SENT packet successfully" << std::endl;
		}
	}
}

void NetworkManager::receiveDataTCP(ChatData& chatData)
{
	sf::Packet packet;

	if (socketTCP->receive(packet) == sf::Socket::Done)
	{
		std::cout << "RECEIVED packet successfully" << std::endl;

		if (packet >> chatData.userName >> chatData.messageBuffer)
		{
			std::cout << "UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
		}
	}
}

void NetworkManager::sendDataUDP(PlayerData& playerData)
{

}

void NetworkManager::receiveDataUDP()
{

}