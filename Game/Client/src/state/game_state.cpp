//@BridgetACasey

#include "context.h"
#include "game_state.h"
#include <iostream>

GameState::GameState()
{
	tickRate = 1000.0f / 64.0f;
	latency = 0.0f;
	
	elapsedTime = 10000.0f;
	lastTime = 0.0f;
	
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

	if (!playerTexture->loadFromFile("assets/player-sprite.png"))
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

	if (!arial.loadFromFile("assets/arial.ttf"))
	{
		printf("could not load font");
	}

	diagnosticText.setFont(arial);
	diagnosticText.setCharacterSize(24);
	diagnosticText.setFillColor(sf::Color::White);
	diagnosticText.setPosition(sf::Vector2f(25.0f, 25.0f));
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

	updateGameState(deltaTime);

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

	updatePlayerPositions(deltaTime);

	if (chatButton->isClicked())
	{
		playing = false;
	}

	if (playButton->isClicked())
	{
		playing = true;
	}

	checkQuit();

	diagnosticText.setString(sf::String("Tick Rate: " + std::to_string(tickRate) + "ms   Latency: " + std::to_string((int)latency) + "ms"));

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

	context->getWindowManager()->render(diagnosticText);

	context->getWindowManager()->endRender();
}

void GameState::updateGameState(float deltaTime)
{
	sf::Packet updatePacket;

	if (context->getNetworkManager()->getSocketTCP()->receive(updatePacket) == sf::Socket::Done)
	{
		if (updatePacket.getDataSize() == (sizeof(int) * 2))
		{
			updatePlayerCount(updatePacket);
		}

		else
		{
			updateChatLog(updatePacket, deltaTime);
		}
	}
}

void GameState::checkQuit()
{
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
}

void GameState::updatePlayerCount(sf::Packet& receivedPacket)
{
	int clientFlag;
	int clientPort;

	if (receivedPacket >> clientFlag >> clientPort)
	{
		if (clientPort != player->getPlayerID())
		{
			if (clientFlag == -1)
			{
				createPlayerInstance(clientPort);
			}

			else if (clientFlag == -2)
			{
				removePlayerInstance(clientPort);
			}

			tickRate = 1000.0f / (64.0f / (1 + otherPlayers.size()));
		}
	}
}

void GameState::createPlayerInstance(int id)
{
	for (Player* other : otherPlayers)
	{
		if (other->getPlayerID() == id)
		{
			return;
		}
	}
	
	Player* newPlayer = new Player(context->getInputManager());

	newPlayer->setPlayerID(id);
	newPlayer->setPosition(sf::Vector2f(575.0f, 300.0f));
	newPlayer->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/player-sprite.png"))
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

	if (sendUpdate(tickRate))
	{
		context->getNetworkManager()->sendDataUDP(playerData);
	}
	
	//Receive player data from the server
	PlayerData receivePlayer;

	context->getNetworkManager()->receiveDataUDP(receivePlayer);

	for (Player* otherPlayer : otherPlayers)
	{
		otherPlayer->interpolate(deltaTime);

		if (validateData(receivePlayer))
		{
			if (otherPlayer->getPlayerID() == receivePlayer.id)
			{
				otherPlayer->setVelocity(receivePlayer.velX, receivePlayer.velY);
				otherPlayer->setNextPosition(receivePlayer.nextPosX, receivePlayer.nextPosY);
			}
		}
	}

	if (player->getPlayerID() == receivePlayer.id)
	{
		latency = (elapsedTime - receivePlayer.time);
	}
}

void GameState::updateChatLog(sf::Packet& receivedPacket, float deltaTime)
{
	ChatData receiveChat;

	if (receivedPacket >> receiveChat.userName >> receiveChat.messageBuffer)
	{
		if (!validateData(receiveChat))
		{
			return;
		}

		std::cout << "(TCP) UNPACKED data successfully - chat message: " << receiveChat.messageBuffer.toAnsiString() << std::endl;
		if (receiveChat.messageBuffer.getSize() > 0)
		{
			chatManager->addNewMessage(receiveChat.userName, receiveChat.messageBuffer);
		}
	}
}

bool GameState::sendUpdate(float period)
{
	if ((elapsedTime - lastTime) >= period)
	{
		lastTime = elapsedTime;

		return true;
	}

	return false;
}

bool GameState::validateData(ChatData& chatData)
{
	if (chatData.userName.getSize() > 16)
		return false;
	if (chatData.messageBuffer.getSize() > 48)
		return false;

	return true;
}

bool GameState::validateData(PlayerData& playerData)
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