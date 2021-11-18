//@BridgetACasey

#include "context.h"
#include "game_state.h"

#define SERVERIP "127.0.0.1"

#define SERVERPORT 5555

GameState::GameState()
{
	player = nullptr;
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

	if (!arial.loadFromFile("assets/arial.ttf"))
	{
		printf("could not load font");
	}

	chatText.setFont(arial);
	chatText.setCharacterSize(24);
	chatText.setFillColor(sf::Color::White);
	chatText.setPosition(sf::Vector2f(50.0f, 600.0f));

	playButton = new UIButton(context->getInputManager(), "assets/play-active.png", "assets/play-idle.png");
	playButton->setPosition(sf::Vector2f(1000.0f, 400.0f));
	playButton->setSize(sf::Vector2f(75.0f, 75.0f));

	chatButton = new UIButton(context->getInputManager(), "assets/chat-active.png", "assets/chat-idle.png");
	chatButton->setPosition(sf::Vector2f(1000.0f, 500.0f));
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

	if (context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Escape) == InputStatus::PRESSED)
	{
		return false;
	}

	if (playing)
	{
		player->checkBounds((float)context->getWindowManager()->getWindow()->getSize().x, (float)context->getWindowManager()->getWindow()->getSize().y);
		player->update(deltaTime);

		PlayerData playerData;

		playerData.id = 1;
		playerData.posX = player->getPosition().x;
		playerData.posY = player->getPosition().y;
		playerData.velX = player->getVelocity().x;
		playerData.velY = player->getVelocity().y;

		sendPlayerData(playerData);
	}

	else
	{
		chatManager->updateMessageStream(deltaTime);
		chatText.setString(chatManager->getMessage());
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

	context->getWindowManager()->render(*player);
	context->getWindowManager()->render(chatText);
	context->getWindowManager()->render(*chatButton);
	context->getWindowManager()->render(*playButton);

	context->getWindowManager()->endRender();
}

void GameState::sendPlayerData(PlayerData& playerData)
{
	sf::Packet packet;

	packet << playerData.id << playerData.posX << playerData.posY << playerData.velX << playerData.velY;

	context->getUDP()->send(packet, SERVERIP, SERVERPORT);
}