//@BridgetACasey

#include "player.h"
#include "input/input_manager.h"

Player::Player(InputManager* input)
{
	inputManager = input;

	velocity = sf::Vector2f(0.0f, 0.0f);
	speed = 250.0f;

	playerID = 0;
}

Player::~Player()
{

}

const void Player::update(float deltaTime)
{
	move(deltaTime);
}

void Player::checkBounds(float screenWidth, float screenHeight)
{
	if (getPosition().x < 0.0f)
		setPosition(0.0f, getPosition().y);

	else if (getPosition().x > (screenWidth - getSize().x))
		setPosition(screenWidth - getSize().x, getPosition().y);

	if (getPosition().y < 0.0f)
		setPosition(getPosition().x, 0.0f);

	else if (getPosition().y > (screenHeight - getSize().y))
		setPosition(getPosition().x, screenHeight - getSize().y);
}

void Player::move(float deltaTime)
{
	velocity = sf::Vector2f(0.0f, 0.0f);

	if (inputManager->getKeyStatus(sf::Keyboard::Key::W) == InputStatus::PRESSED)
		velocity.y = -speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::A) == InputStatus::PRESSED)
		velocity.x = -speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::S) == InputStatus::PRESSED)
		velocity.y = speed;
	if (inputManager->getKeyStatus(sf::Keyboard::Key::D) == InputStatus::PRESSED)
		velocity.x = speed;

	setPosition(getPosition().x + (velocity.x * deltaTime), getPosition().y + (velocity.y * deltaTime));
}