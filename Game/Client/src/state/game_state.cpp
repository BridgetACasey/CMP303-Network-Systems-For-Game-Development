//@BridgetACasey

#include "context.h"
#include "game_state.h"
#include <iostream>

GameState::GameState()
{
	tickRate = 1000.0f / 64.0f;	//By default, the tick rate is approximately 15ms
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

	//Set the player's form of ID to the current UDP port
	player->setPlayerPort(context->getNetworkManager()->getSocketUDP()->getLocalPort());
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

	modeText.setFont(arial);
	modeText.setStyle(sf::Text::Bold);
	modeText.setCharacterSize(24);
	modeText.setFillColor(sf::Color::White);
	modeText.setPosition(sf::Vector2f(25.0f, 65.0f));
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

	updateGameState(deltaTime);

	if (!playing)	//If not controlling an avatar, update the chat's input stream
	{
		chatManager->updateMessageStream(deltaTime);

		//Send the message to the server once ENTER has been pressed
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

	checkQuit();

	//Update UI elements
	if (chatButton->isClicked())
	{
		playing = false;
	}

	if (playButton->isClicked())
	{
		playing = true;
	}

	diagnosticText.setString(sf::String("Tick Rate: " + std::to_string(tickRate) + "ms   Latency: " + std::to_string((int)latency) + "ms"));

	if (playing)
	{
		modeText.setString(sf::String("MODE: PLAYING"));
	}

	else
	{
		modeText.setString(sf::String("MODE: CHATTING"));
	}

	return running;
}

void GameState::render()
{
	context->getWindowManager()->beginRender();

	//Render all other connected clients' avatars, if any exist
	for (Player* otherPlayer : otherPlayers)
	{
		if (otherPlayer)
		{
			context->getWindowManager()->render(*otherPlayer);
		}
	}

	//Render this client's player avatar on top of the others
	context->getWindowManager()->render(*player);

	//Render the chat messages and input box
	for (sf::Text text : chatManager->getChatMessages())
	{
		context->getWindowManager()->render(text);
	}

	context->getWindowManager()->render(*chatBar);
	context->getWindowManager()->render(*chatManager->getInputText());

	//Render other UI elements
	context->getWindowManager()->render(*chatButton);
	context->getWindowManager()->render(*playButton);

	context->getWindowManager()->render(diagnosticText);
	context->getWindowManager()->render(modeText);

	context->getWindowManager()->endRender();
}

void GameState::updatePlayerPositions(float deltaTime)
{
	//Update active player position
	player->checkBounds((float)context->getWindowManager()->getWindow()->getSize().x, (float)context->getWindowManager()->getWindow()->getSize().y);
	player->update(deltaTime);

	if (playing)	//If in 'play' mode, listen for keyboard input from the user
	{
		player->getUserInput();
	}

	if (sendUpdate(tickRate))
	{
		//Send player data by UDP
		PlayerData playerData;

		playerData.time = elapsedTime;
		playerData.port = player->getPlayerPort();
		playerData.posX = player->getPosition().x;
		playerData.posY = player->getPosition().y;
		playerData.nextPosX = player->getNextPosition().x;
		playerData.nextPosY = player->getNextPosition().y;
		playerData.velX = player->getVelocity().x;
		playerData.velY = player->getVelocity().y;

		context->getNetworkManager()->sendDataUDP(playerData);
	}

	//Receive player data from the server
	PlayerData receivePlayer;

	if (context->getNetworkManager()->receiveDataUDP(receivePlayer))
	{
		if (player->getPlayerPort() == receivePlayer.port)
		{
			latency = (elapsedTime - receivePlayer.time);
		}
	}

	for (Player* otherPlayer : otherPlayers)
	{
		otherPlayer->interpolate(deltaTime);

		if (otherPlayer->getPlayerPort() == receivePlayer.port)
		{
			otherPlayer->setVelocity(receivePlayer.velX, receivePlayer.velY);
			otherPlayer->setNextPosition(receivePlayer.nextPosX, receivePlayer.nextPosY);
		}
	}
}

void GameState::updateChatLog(sf::Packet& receivedPacket, float deltaTime)
{
	ChatData receiveChat;

	if (context->getNetworkManager()->receiveChatData(receiveChat, receivedPacket))
	{
		if (receiveChat.messageBuffer.getSize() > 0)
		{
			chatManager->addNewMessage(receiveChat.userName, receiveChat.messageBuffer);
		}
	}
}

//Returns true if it has been more than the specified time since an update was sent to the server
bool GameState::sendUpdate(float period)
{
	if ((elapsedTime - lastTime) >= period)
	{
		lastTime = elapsedTime;

		return true;
	}

	return false;
}

void GameState::updateGameState(float deltaTime)
{
	sf::Packet updatePacket;

	if (context->getNetworkManager()->getSocketTCP()->receive(updatePacket) == sf::Socket::Done)
	{
		if (updatePacket.getDataSize() == sizeof(int) + sizeof(sf::Uint16))
		{
			//A client has connected to or disconnected from the server, update relevant player data
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
		if (context->getNetworkManager()->requestDisconnection())
		{
			running = false;
		}
	}
}

void GameState::updatePlayerCount(sf::Packet& receivedPacket)
{
	int clientFlag;
	sf::Uint16 clientPort;

	if (receivedPacket >> clientFlag >> clientPort)
	{
		if (clientPort != player->getPlayerPort())
		{
			if (clientFlag == -1)
			{
				createPlayerInstance(clientPort);
			}

			else if (clientFlag == -2)
			{
				removePlayerInstance(clientPort);
			}
			
			//Update tick rate to accommodate for how many clients are connected to the server
			tickRate = 1000.0f / (64.0f / (1 + otherPlayers.size()));
		}
	}
}

void GameState::createPlayerInstance(sf::Uint16 port)
{
	for (Player* other : otherPlayers)
	{
		if (other->getPlayerPort() == port)
		{
			//If an instance of the current player has already been created, avoid creating a duplicate
			return;
		}
	}
	
	Player* newPlayer = new Player(context->getInputManager());

	newPlayer->setPlayerPort(port);
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

void GameState::removePlayerInstance(sf::Uint16 port)
{
	for (int i = 0; i < otherPlayers.size(); i++)
	{
		if (otherPlayers.at(i)->getPlayerPort() == port)
		{
			otherPlayers.erase(otherPlayers.begin() + i);
		}
	}

	otherPlayers.shrink_to_fit();
}