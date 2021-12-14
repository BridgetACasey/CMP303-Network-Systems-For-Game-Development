//@BridgetACasey

#include <iostream>

#include "network_manager.h"

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

const sf::Uint16 serverPortTCP = 5555;
const sf::Uint16 serverPortUDP = 4444;

NetworkManager::NetworkManager()
{
	serverAddress = "192.168.0.18";

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

bool NetworkManager::requestConnection(const sf::String& address)
{
	serverAddress = sf::IpAddress(address);

	if(socketTCP->connect(serverAddress, serverPortTCP) != sf::TcpSocket::Done)
	{
		std::cout << "FAILED TO CONNECT! - The server is either full or not running" << std::endl;
	}
	
	else if(socketTCP->connect(serverAddress, serverPortTCP) == sf::TcpSocket::Done)
	{
		socketTCP->setBlocking(false);

		sf::Packet packet;

		sf::Uint32 address = sf::IpAddress::getLocalAddress().toInteger();

		packet << socketUDP->getLocalPort() << address;

		if (socketTCP->send(packet) == sf::Socket::Done)
		{
			std::cout << "Connection established on port " << socketUDP->getLocalPort() << std::endl;
		}

		return true;
	}

	return false;
}

bool NetworkManager::sendDataTCP(ChatData& chatData, int quitFlag, sf::Uint16 playerPort)
{
	bool status = false;

	sf::Packet packet;

	if (quitFlag == -1)
	{
		if (packet << quitFlag)
		{
			std::cout << "(TCP) PACKED data successfully - Request to quit" << std::endl;
		}
	}

	else
	{
		if (packet << chatData)
		{
			std::cout << "(TCP) PACKED data successfully - Chat message" << std::endl;
		}
	}

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

	return status;
}

bool NetworkManager::receiveDataTCP(ChatData& chatData, int& quitFlag, sf::Uint16& playerPort)
{
	bool status = false;
	
	sf::Packet packet;

	sf::Socket::Status socketStatus = socketTCP->receive(packet);

	switch (socketStatus)
	{
	case sf::Socket::Done:
		std::cout << "(TCP) RECEIVED packet successfully" << std::endl;

		if (packet.getDataSize() == sizeof(int))
		{
			//A client has connected to or disconnected from the server, update relevant player data
			if (packet >> quitFlag)
			{
				if (quitFlag == -2)
				{
					std::cout << "Disconnected from server, closing application..." << std::endl;
					status = true;
				}
			}
		}

		else if (packet.getDataSize() == sizeof(int) + sizeof(sf::Uint16))
		{
			//A client has connected to or disconnected from the server, update relevant player data
			if (packet >> quitFlag >> playerPort)
			{
				std::cout << "New user has connected on port " << std::to_string(playerPort) << std::endl;
				status = true;
			}
		}

		else
		{
			if (packet >> chatData)
			{
				if (validateData(chatData))
				{
					std::cout << "(TCP) UNPACKED data successfully - chat message: " << chatData.messageBuffer.toAnsiString() << std::endl;
					status = true;
				}
			}
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

	for (PlayerData& player : previousPlayerData)
	{
		sf::String name = "P_" + std::to_string(player.port);

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
bool NetworkManager::validateData(PlayerData& playerData)
{
	int playerIndex = -1;

	if (playerData.time < 0.0f)
		return false;
	if (playerData.port < 0 || playerData.port > 65535)
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
	if (playerData.disX < -300.0f || playerData.disX > 300.0f)
		return false;
	if (playerData.disY < -300.0f || playerData.disY > 300.0f)
		return false;

	for (int i = 0; i < previousPlayerData.size(); i++)
	{
		if (previousPlayerData.at(i).port == playerData.port)
		{
			playerIndex = i;

			if (playerData.disX != 0.0f && playerData.disY != 0.0f)
			{
				float offset = 150.0f;

				//Position data received was technically valid but unrealistic, recalculate next position
				float lastDisX = abs(previousPlayerData.at(i).nextPosX - previousPlayerData.at(i).posX);
				float lastDisY = abs(previousPlayerData.at(i).nextPosY - previousPlayerData.at(i).posY);
				float newDisX = abs(playerData.nextPosX - playerData.posX);
				float newDisY = abs(playerData.nextPosY - playerData.posY);

				if (newDisX > (lastDisX + abs(playerData.disX) + offset))
				{
					playerData.nextPosX = previousPlayerData.at(i).nextPosX;
				}

				if (newDisY > (lastDisY + abs(playerData.disY) + offset))
				{
					playerData.nextPosY = previousPlayerData.at(i).nextPosY;
				}
			}
		}
	}

	if (playerIndex == -1)
	{
		return false;
	}

	previousPlayerData.at(playerIndex) = playerData;

	return true;
}

void NetworkManager::insertPlayerData(PlayerData& playerData)
{
	previousPlayerData.push_back(playerData);
}