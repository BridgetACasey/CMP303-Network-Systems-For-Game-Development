//@BridgetACasey

#include "context.h"
#include "game_state.h"

GameState::GameState()
{
	player = nullptr;
	chatBar = nullptr;
	chatManager = nullptr;
	chatButton = nullptr;
	playButton = nullptr;

	playing = true;
}

GameState::~GameState()
{

}

void GameState::setup()
{
	chatManager = new ChatManager(context->getInputManager());
	
	player = new Player(context->getInputManager());

	player->setPosition(sf::Vector2f(575.0f, 300.0f));
	player->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		printf("could not load texture");
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
	context->getInputManager()->update(deltaTime);

	updatePlayerPositions(deltaTime);

	updateChatLog(deltaTime);

	if (context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Escape) == InputStatus::PRESSED)
	{
		return false;
	}

	if (chatButton->isClicked())
	{
		playing = false;
	}

	if (playButton->isClicked())
	{
		playing = true;
	}

	return true;
}

void GameState::render()
{
	context->getWindowManager()->beginRender();

	for (GameObject* otherPlayer : otherPlayers)
	{
		context->getWindowManager()->render(*otherPlayer);
	}

	context->getWindowManager()->render(*player);

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

void GameState::createNewPlayerInstance()
{
	GameObject* otherPlayer = new GameObject();

	otherPlayer->setPosition(sf::Vector2f(575.0f, 300.0f));
	otherPlayer->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		printf("could not load texture");
	}

	otherPlayer->setTexture(playerTexture);

	otherPlayers.push_back(otherPlayer);
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

	playerData.id = 1;
	playerData.posX = player->getPosition().x;
	playerData.posY = player->getPosition().y;
	playerData.spritePath = "assets/potatolizard.png";

	context->getNetworkManager()->sendDataUDP(playerData);

	//Receive player data from the server
	PlayerData receivePlayer;
	context->getNetworkManager()->receiveDataUDP(receivePlayer);

	if (receivePlayer.total > (otherPlayers.size() + 1))
	{
		createNewPlayerInstance();
	}

	else if (receivePlayer.total < (otherPlayers.size() + 1))
	{
		//remove the player that has disconnected
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
			sendChat.userName = "The_Player45";
			sendChat.messageBuffer = chatManager->getInputText()->getString();
			context->getNetworkManager()->sendDataTCP(sendChat);
		}
	}
}