//@BridgetACasey

#include "player.h"
#include "input/input_manager.h"

Player::Player(InputManager* input)
{
	inputManager = input;

	speed = 250.0f;
	velocity = sf::Vector2f(speed, speed);

	playerPort = 0;
}

Player::~Player()
{

}

const void Player::update(float deltaTime)
{
	//Update player position relative to current velocity and delta time
	setPosition(getPosition().x + (velocity.x * deltaTime), getPosition().y + (velocity.y * deltaTime));
}

void Player::getUserInput()
{
	//Altering the player's velocity based on which key has been pressed
	if (inputManager->getKeyStatus(sf::Keyboard::Key::W) == InputStatus::PRESSED)
		velocity.y = -speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::A) == InputStatus::PRESSED)
		velocity.x = -speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::S) == InputStatus::PRESSED)
		velocity.y = speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::D) == InputStatus::PRESSED)
		velocity.x = speed;
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