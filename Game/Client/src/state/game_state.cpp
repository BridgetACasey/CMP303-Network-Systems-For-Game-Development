//@BridgetACasey

#include "context.h"
#include "game_state.h"
#include "SFML/Graphics/Texture.hpp"

GameState::GameState()
{
	player = new Player();

	player->setPosition(sf::Vector2f(575.0f, 300.0f));
	player->setSize(sf::Vector2f(50.0f, 50.0f));

	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile("assets/potatolizard.png"))
	{
		printf("could not load texture");
	}

	player->setTexture(playerTexture);
}

GameState::~GameState()
{

}

void GameState::setup()
{

}

void GameState::onEnter()
{

}

void GameState::onExit()
{

}

void GameState::handleInput()
{

}

bool GameState::update(float deltaTime)
{
	context->getInputManager()->update(deltaTime);

	return true;
}

void GameState::render()
{
	context->getWindowManager()->beginRender();

	context->getWindowManager()->render(*player);

	context->getWindowManager()->endRender();
}