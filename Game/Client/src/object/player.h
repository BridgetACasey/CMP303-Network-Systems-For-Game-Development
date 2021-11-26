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

	inline sf::Vector2f& getVelocity() { return velocity; }

	inline void setPlayerID(int id) { playerID = id; }
	inline int getPlayerID() const { return playerID; }

private:
	void move(float deltaTime);

	InputManager* inputManager;

	sf::Vector2f velocity;
	float speed;

	int playerID;
};