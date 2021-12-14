//@BridgetACasey

#include "player.h"
#include "input/input_manager.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

Player::Player(InputManager* input)
{
	inputManager = input;

	speed = 250.0f;
	velocity = sf::Vector2f(speed, speed);

	playerPort = 0;

	ghost = nullptr;
	namePlate = nullptr;

	activePlayer = false;

	constantMove = true;

	elapsedTime = 0.0f;
	lastUpdateTime = 0.0f;

	timeout = false;
}

Player::~Player()
{
	if (ghost)
	{
		delete ghost;
	}

	if (namePlate)
	{
		delete namePlate;
	}
}

const void Player::update(float deltaTime)
{
	if (activePlayer)
	{
		//Update player position relative to current velocity and delta time
		setPosition(getPosition().x + (velocity.x * deltaTime), getPosition().y + (velocity.y * deltaTime));
	}

	if (ghost)
	{
		ghost->setPosition(nextPosition.x + (velocity.x * deltaTime), nextPosition.y + (velocity.y * deltaTime));
	}

	if (namePlate)
	{
		namePlate->setPosition(sf::Vector2f(getPosition().x - 25.0f, getPosition().y - 30.0f));
	}

	if ((elapsedTime - lastUpdateTime) >= 15000.0f)
	{
		lastUpdateTime = elapsedTime;

		timeout = true;
	}
}

void Player::getUserInput()
{
	int keysPressed = 0;

	//Altering the player's velocity based on which key has been pressed
	if (inputManager->getKeyStatus(sf::Keyboard::Key::W) == InputStatus::PRESSED)
	{
		velocity.y = -speed;
		keysPressed++;
	}

	if (inputManager->getKeyStatus(sf::Keyboard::Key::A) == InputStatus::PRESSED)
	{
		velocity.x = -speed;
		keysPressed++;
	}

	if (inputManager->getKeyStatus(sf::Keyboard::Key::S) == InputStatus::PRESSED)
	{
		velocity.y = speed;
		keysPressed++;
	}

	if (inputManager->getKeyStatus(sf::Keyboard::Key::D) == InputStatus::PRESSED)
	{
		velocity.x = speed;
		keysPressed++;
	}

	if (!constantMove)
	{
		if (keysPressed == 0)
		{
			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}
	}
}

void Player::checkBounds(float screenWidth, float screenHeight)
{
	//If the player has collided with the bounds of the window, reset their position and begin moving in the opposite direction
	if (getPosition().x < 0.0f)
	{
		setPosition(0.0f, getPosition().y);
		velocity.x *= -1.0f;
	}

	else if (getPosition().x > (screenWidth - getSize().x))
	{
		setPosition(screenWidth - getSize().x, getPosition().y);
		velocity.x *= -1.0f;
	}

	if (getPosition().y < 0.0f)
	{
		setPosition(getPosition().x, 0.0f);
		velocity.y *= -1.0f;
	}

	else if (getPosition().y > (screenHeight - getSize().y))
	{
		setPosition(getPosition().x, screenHeight - getSize().y);
		velocity.y *= -1.0f;
	}
}

void Player::setPlayerTexture(const std::string& filePath)
{
	sf::Texture* playerTexture = new sf::Texture();

	if (!playerTexture->loadFromFile(filePath))
	{
		std::cout << "could not load texture" << filePath << std::endl;

		delete playerTexture;

		return;
	}

	setTexture(playerTexture);
}

void Player::createGhost(const std::string& filePath)
{
	ghost = new GameObject();

	sf::Texture* ghostTexture = new sf::Texture();

	if (!ghostTexture->loadFromFile(filePath))
	{
		std::cout << "could not load texture" << filePath << std::endl;

		delete ghostTexture;

		delete ghost;

		return;
	}

	ghost->setTexture(ghostTexture);
	ghost->setSize(sf::Vector2f(50.0f, 50.0f));
	ghost->setPosition(getPosition().x, getPosition().y);
}