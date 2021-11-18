//@BridgetACasey

#pragma once

#include "game_object.h"

class InputManager;

class Player : public GameObject
{
public:
	Player(InputManager* input);
	~Player();

	const void update(float deltaTime) override;

	void checkBounds(float screenWidth, float screenHeight);

private:
	void move(float deltaTime);

	InputManager* inputManager;

	sf::Vector2f velocity;
	float speed;
};