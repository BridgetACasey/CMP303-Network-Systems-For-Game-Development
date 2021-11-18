//@BridgetACasey

#include "context.h"
#include "game_state.h"

GameState::GameState()
{
	player = nullptr;
}

GameState::~GameState()
{

}

void GameState::setup()
{
	player = new Player(context->getInputManager());

	player->setPosition(sf::Vector2f(575.0f, 300.0f));
	player->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		printf("could not load texture");
	}

	player->setTexture(playerTexture);
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

	player->checkBounds((float)context->getWindowManager()->getWindow()->getSize().x, (float)context->getWindowManager()->getWindow()->getSize().y);
	player->update(deltaTime);

	return true;
}

void GameState::render()
{
	context->getWindowManager()->beginRender();

	context->getWindowManager()->render(*player);

	context->getWindowManager()->endRender();
}