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
		socketUDP->setBlocking(false);
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
	
	else
	{
		socketTCP->setBlocking(false);

		sf::Packet packet;

		packet << socketUDP->getLocalPort();

		if (socketTCP->send(packet) == sf::Socket::Done)
		{
			std::cout << "Connection established on port " << socketUDP->getLocalPort() << std::endl;
		}
	}
}

void NetworkManager::requestDisconnection()
{
	socketTCP->disconnect();
}

bool NetworkManager::sendDataTCP(ChatData& chatData)
{
	bool status = false;

	sf::Packet packet;

	if (packet << chatData.userName << chatData.messageBuffer)
	{
		std::cout << "(TCP) PACKED data successfully" << std::endl;

		sf::Socket::Status socketStatus = socketTCP->send(packet);

		switch (socketStatus)
		{
		case sf::Socket::Done:
			std::cout << "(TCP) SENT packet successfully" << std::endl;
			status = true;
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

	return status;
}

bool NetworkManager::receiveDataTCP(ChatData& chatData)
{
	bool status = false;
	
	sf::Packet packet;

	sf::Socket::Status socketStatus = socketTCP->receive(packet);

	switch (socketStatus)
	{
	case sf::Socket::Done:
		std::cout << "(TCP) RECEIVED packet successfully" << std::endl;
		if (packet >> chatData.userName >> chatData.messageBuffer)
		{
			if(validateData(chatData))
			std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
			status = true;
		}
		break;

	case sf::Socket::NotReady:
		//std::cout << "(TCP) NOT READY to receive packet" << std::endl;
		break;

	case sf::Socket::Partial:
		std::cout << "(TCP) PARTIAL packet received" << std::endl;
		packet >> pendingChatData.userName >> pendingChatData.messageBuffer;

		if (sizeof(pendingChatData) + packet.getDataSize() == sizeof(ChatData))
		{
			chatData = pendingChatData;

			pendingChatData.userName = "";
			pendingChatData.messageBuffer = "";

			status = true;
		}
		break;

	case sf::Socket::Error:
		std::cout << "(TCP) ERROR receiving packet" << std::endl;
		break;

	default:
		break;
	}

	return status;
}

bool NetworkManager::sendDataUDP(PlayerData& playerData)
{
	bool status = false;
	
	sf::Packet packet;

	if (packet << playerData.time << playerData.id << playerData.posX << playerData.posY << playerData.nextPosX << playerData.nextPosY << playerData.velX << playerData.velY)
	{
		//std::cout << "(UDP) PACKED data successfully" << std::endl;

		sf::Socket::Status socketStatus = socketUDP->send(packet, serverAddress, serverPortUDP);

		switch (socketStatus)
		{
		case sf::Socket::Done:
			//std::cout << "(UDP) SENT packet successfully" << std::endl;
			status = true;
			break;

		case sf::Socket::Partial:
			std::cout << "(UDP) PARTIAL packet sent" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "(UDP) ERROR sending packet" << std::endl;
			break;

		default:
			break;
		}
	}

	return status;
}

bool NetworkManager::receiveDataUDP(PlayerData& playerData)
{
	bool status = false;
	
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;

	sf::Socket::Status socketStatus = socketUDP->receive(packet, address, port);

	switch (socketStatus)
	{
	case sf::Socket::Done:
		//std::cout << "(UDP) SENT packet successfully" << std::endl;
		if (packet >> playerData.time >> playerData.id >> playerData.posX >> playerData.posY >> playerData.nextPosX >> playerData.nextPosY >> playerData.velX >> playerData.velY)
		{
			if(validateData(playerData))
			status = true;
		}
		break;

	case sf::Socket::NotReady:
		//std::cout << "(UDP) NOT READY to receive packet" << std::endl;
		break;

	case sf::Socket::Partial:
		std::cout << "(UDP) PARTIAL packet received" << std::endl;

		packet >> pendingPlayerData.time >> pendingPlayerData.id >> pendingPlayerData.posX >> pendingPlayerData.posY >> pendingPlayerData.nextPosX >>
			pendingPlayerData.nextPosY >> pendingPlayerData.velX >> pendingPlayerData.velY;

		if (sizeof(pendingPlayerData) + packet.getDataSize() == sizeof(PlayerData))
		{
			playerData = pendingPlayerData;

			pendingPlayerData.id = 0;
			pendingPlayerData.time = 0.0f;
			pendingPlayerData.posX = 0.0f;
			pendingPlayerData.posY = 0.0f;
			pendingPlayerData.nextPosX = 0.0f;
			pendingPlayerData.nextPosY = 0.0f;
			pendingPlayerData.velX = 0.0f;
			pendingPlayerData.velY = 0.0f;

			status = true;
		}
		break;

	case sf::Socket::Error:
		std::cout << "(UDP) ERROR receiving packet" << std::endl;
		break;

	default:
		break;
	}

	return status;
}

bool NetworkManager::validateData(ChatData& chatData)
{
	if (chatData.userName.getSize() > 16)
		return false;
	if (chatData.messageBuffer.getSize() > 48)
		return false;

	return true;
}

bool NetworkManager::validateData(PlayerData& playerData)
{
	if (playerData.id < 0)
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