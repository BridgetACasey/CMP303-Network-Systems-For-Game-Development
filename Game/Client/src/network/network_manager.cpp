//@BridgetACasey

#include <iostream>

#include "network_manager.h"

//Operator overloads for reading and writing between packets and frequently used struct types
sf::Packet operator << (sf::Packet& packet, const PlayerData& data)
{
	return packet << data.time << data.port << data.posX << data.posY << data.nextPosX << data.nextPosY << data.velX << data.velY;
}

sf::Packet operator >> (sf::Packet& packet, PlayerData& data)
{
	return packet >> data.time >> data.port >> data.posX >> data.posY >> data.nextPosX >> data.nextPosY >> data.velX >> data.velY;
}

sf::Packet operator << (sf::Packet& packet, const ChatData& data)
{
	return packet << data.userName << data.messageBuffer;
}

sf::Packet operator >> (sf::Packet& packet, ChatData& data)
{
	return packet >> data.userName >> data.messageBuffer;
}

const sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
const sf::Uint16 serverPortTCP = 5555;
const sf::Uint16 serverPortUDP = 4444;

NetworkManager::NetworkManager()
{
	socketTCP = new sf::TcpSocket();
	socketUDP = new sf::UdpSocket();

	bindUDP();
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::bindUDP()
{
	//Pick any available port on the machine and bind to it
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

bool NetworkManager::requestConnection()
{
	if(socketTCP->connect(serverAddress, serverPortTCP) != sf::TcpSocket::Done)
	{
		std::cout << "FAILED TO CONNECT! - The server is either full or not running" << std::endl;
	}
	
	else if(socketTCP->connect(serverAddress, serverPortTCP) == sf::TcpSocket::Done)
	{
		socketTCP->setBlocking(false);

		sf::Packet packet;

		packet << socketUDP->getLocalPort();

		if (socketTCP->send(packet) == sf::Socket::Done)
		{
			std::cout << "Connection established on port " << socketUDP->getLocalPort() << std::endl;
		}

		return true;
	}

	return false;
}

void NetworkManager::requestDisconnection()
{
	socketTCP->disconnect();
}

bool NetworkManager::sendDataTCP(ChatData& chatData)
{
	bool status = false;

	sf::Packet packet;

	if (packet << chatData)
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
		if (packet >> chatData)
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
		packet >> pendingChatData;

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

bool NetworkManager::receiveChatData(ChatData& chatData, sf::Packet& receivedPacket)
{
	if (receivedPacket >> chatData)
	{
		if (validateData(chatData))
		{
			std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;

			return true;
		}
	}

	return false;
}

bool NetworkManager::sendDataUDP(PlayerData& playerData)
{
	bool status = false;
	
	sf::Packet packet;

	if (packet << playerData)
	{
		//std::cout << "(UDP) PACKED data successfully" << std::endl;

		sf::Socket::Status socketStatus = socketUDP->send(packet, serverAddress, serverPortUDP);

		switch (socketStatus)
		{
		case sf::Socket::Done:
			//std::cout << "(UDP) SENT packet successfully" << std::endl;
			status = true;
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
		if (packet >> playerData)
		{
			if(validateData(playerData))
			status = true;
		}
		break;

	case sf::Socket::NotReady:
		//std::cout << "(UDP) NOT READY to receive packet" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "(UDP) ERROR receiving packet" << std::endl;
		break;

	default:
		break;
	}

	return status;
}

//Checking that received packets contain sensible data, otherwise they have likely been tampered with
bool NetworkManager::validateData(ChatData& chatData)
{
	if (chatData.userName.getSize() > 16)
		return false;
	if (chatData.messageBuffer.getSize() > 48)
		return false;

	return true;
}

//Checking that received packets contain sensible data, otherwise they have likely been tampered with
bool NetworkManager::validateData(PlayerData& playerData)
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