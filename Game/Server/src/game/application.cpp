//@BridgetACasey

#include "application.h"

#define SERVERIP "127.0.0.1"

#define SERVERPORT 5555

const int MAX_CLIENTS = 1;

Application::Application()
{

}

Application::~Application()
{

}

void Application::run()
{
	bool running = true;
	
	printf("Waiting for client...");

	listener.listen(SERVERPORT);

	selector.add(listener);

	listenForClients();

	while (running)
	{
		//server loop
	}
}

void Application::listenForClients()
{
	while (clients.size() < MAX_CLIENTS)
	{
		if (selector.wait(sf::microseconds(1)));
		{
			if (selector.isReady(listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket();

				if (listener.accept(*client) == sf::Socket::Done)
				{
					clients.push_back(client);

					selector.add(*client);

					printf("Client has joined!");
				}
			}
		}
	}
}

void Application::receivePlayerData()
{
	sf::Packet packet;
}