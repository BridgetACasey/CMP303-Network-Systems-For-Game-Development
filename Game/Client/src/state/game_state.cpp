//@BridgetACasey

#include "context.h"
#include "game_state.h"
#include <iostream>

GameState::GameState()
{
	elapsedTime = 10000;
	lastTime = 0;
	
	player = nullptr;
	chatBar = nullptr;
	chatManager = nullptr;
	chatButton = nullptr;
	playButton = nullptr;

	playing = true;
	running = true;
}

GameState::~GameState()
{

}

void GameState::setup()
{
	chatManager = new ChatManager(context->getInputManager());

	player = new Player(context->getInputManager());

	player->setPlayerID(context->getNetworkManager()->getSocketUDP()->getLocalPort());
	player->setPosition(sf::Vector2f(575.0f, 300.0f));
	player->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		std::cout << "could not load texture" << std::endl;
	}

	player->setTexture(playerTexture);

	chatBar = new GameObject();
	chatBar->setFillColor(sf::Color(0, 0, 0, 64));
	chatBar->setSize(sf::Vector2f(1030.0f, 30.0f));
	chatBar->setPosition(sf::Vector2f(30.0f, 615.0f));

	playButton = new UIButton(context->getInputManager(), "assets/play-active.png", "assets/play-idle.png");
	playButton->setPosition(sf::Vector2f(1100.0f, 475.0f));
	playButton->setSize(sf::Vector2f(75.0f, 75.0f));

	chatButton = new UIButton(context->getInputManager(), "assets/chat-active.png", "assets/chat-idle.png");
	chatButton->setPosition(sf::Vector2f(1100.0f, 575.0f));
	chatButton->setSize(sf::Vector2f(75.0f, 75.0f));
}

void GameState::onEnter()
{
	if (firstSetup)
	{
		setup();
	}
}

void GameState::onExit()
{

}

bool GameState::update(float deltaTime)
{
	elapsedTime = (float)getClientTime();

	context->getInputManager()->update(deltaTime);

	updatePlayerPositions(deltaTime);

	updateChatLog(deltaTime);

	if (chatButton->isClicked())
	{
		playing = false;
	}

	if (playButton->isClicked())
	{
		playing = true;
	}

	if (context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Escape) == InputStatus::PRESSED)
	{
		sf::Packet sentPacket;

		int quit = -1;

		if (sentPacket << quit)
		{
			if (context->getNetworkManager()->getSocketTCP()->send(sentPacket) == sf::Socket::Done)
			{
				std::cout << "Requesting to quit" << std::endl;

				context->getNetworkManager()->getSocketTCP()->setBlocking(true);

				sf::Packet receivedPacket;

				if (context->getNetworkManager()->getSocketTCP()->receive(receivedPacket) == sf::Socket::Done)
				{
					if (receivedPacket >> quit)
					{
						if (quit == -2)
						{
							std::cout << "Disconnected from server, closing application..." << std::endl;
							running = false;
						}
					}
				}
			}
		}
	}

	return running;
}

void GameState::render()
{
	context->getWindowManager()->beginRender();

	for (Player* otherPlayer : otherPlayers)
	{
		if (otherPlayer)
		{
			context->getWindowManager()->render(*otherPlayer);
		}
	}

	if (player)
	{
		context->getWindowManager()->render(*player);
	}

	context->getWindowManager()->render(*chatButton);
	context->getWindowManager()->render(*playButton);

	for (sf::Text text : chatManager->getChatMessages())
	{
		context->getWindowManager()->render(text);
	}

	context->getWindowManager()->render(*chatBar);
	context->getWindowManager()->render(*chatManager->getInputText());

	context->getWindowManager()->endRender();
}

void GameState::createPlayerInstance(int id)
{
	Player* newPlayer = new Player(context->getInputManager());

	newPlayer->setPlayerID(id);
	newPlayer->setPosition(sf::Vector2f(575.0f, 300.0f));
	newPlayer->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		std::cout << "could not load texture" << std::endl;
	}

	newPlayer->setTexture(playerTexture);

	otherPlayers.push_back(newPlayer);
}

void GameState::removePlayerInstance(int id)
{
	for (int i = 0; i < otherPlayers.size(); i++)
	{
		if (otherPlayers.at(i)->getPlayerID() == id)
		{
			otherPlayers.erase(otherPlayers.begin() + i);
		}
	}

	otherPlayers.shrink_to_fit();
}

void GameState::updatePlayerPositions(float deltaTime)
{
	if (playing)
	{
		player->checkBounds((float)context->getWindowManager()->getWindow()->getSize().x, (float)context->getWindowManager()->getWindow()->getSize().y);
		player->update(deltaTime);
	}

	//Send player data by UDP
	PlayerData playerData;

	playerData.time = elapsedTime;
	playerData.id = player->getPlayerID();
	playerData.posX = player->getPosition().x;
	playerData.posY = player->getPosition().y;
	playerData.nextPosX = player->getNextPosition().x;
	playerData.nextPosY = player->getNextPosition().y;
	playerData.velX = player->getVelocity().x;
	playerData.velY = player->getVelocity().y;

	if (sendUpdate(100.0f))
	{
		context->getNetworkManager()->sendDataUDP(playerData);
	}
	
	//Receive player data from the server
	PlayerData receivePlayer;

	context->getNetworkManager()->receiveDataUDP(receivePlayer);

	std::cout << "Time: " << elapsedTime << std::endl;

	if (receivePlayer.id > 0 && receivePlayer.id != player->getPlayerID())
	{
		if ((receivePlayer.total > (otherPlayers.size() + 1)))
		{
			createPlayerInstance(receivePlayer.id);
		}

		else if (receivePlayer.total < (otherPlayers.size() + 1))
		{
			removePlayerInstance(receivePlayer.id);
		}
	}

	for (Player* otherPlayer : otherPlayers)
	{
		otherPlayer->interpolate(deltaTime);

		if (otherPlayer->getPlayerID() == receivePlayer.id)
		{
			//otherPlayer->setPosition(sf::Vector2f(receivePlayer.posX, receivePlayer.posY));
			otherPlayer->setVelocity(receivePlayer.velX, receivePlayer.velY);
			otherPlayer->setNextPosition(receivePlayer.nextPosX, receivePlayer.nextPosY);
		}
	}
}

void GameState::updateChatLog(float deltaTime)
{
	ChatData receiveChat;
	context->getNetworkManager()->receiveDataTCP(receiveChat);
	if (receiveChat.messageBuffer.getSize() > 0)
	{
		chatManager->addNewMessage(receiveChat.userName, receiveChat.messageBuffer);
	}

	if (!playing)
	{
		chatManager->updateMessageStream(deltaTime);

		if (context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Enter) == InputStatus::PRESSED)
		{
			context->getInputManager()->setKeyStatus(sf::Keyboard::Key::Enter, InputStatus::NONE);

			ChatData sendChat;
			sendChat.userName = "P_" + std::to_string(context->getNetworkManager()->getSocketUDP()->getLocalPort());
			sendChat.messageBuffer = chatManager->getInputText()->getString();
			context->getNetworkManager()->sendDataTCP(sendChat);
		}
	}
}

bool GameState::sendUpdate(float frequency)
{
	if ((elapsedTime - lastTime) > frequency)
	{
		lastTime = elapsedTime;

		return true;
	}

	return false;
}