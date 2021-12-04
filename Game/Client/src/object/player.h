//@BridgetACasey

#pragma once

#include "game_object.h"

class InputManager;

//Player is a game object that can be controlled by the user
class Player : public GameObject
{
public:
	Player(InputManager* input);
	~Player();

	const void update(float deltaTime) override;
	void getUserInput();

	void checkBounds(float screenWidth, float screenHeight);

	inline sf::Vector2f& getVelocity() { return velocity; }

	inline void setPlayerPort(sf::Uint16 port) { playerPort = port; }
	inline sf::Uint16 getPlayerPort() const { return playerPort; }

private:
	InputManager* inputManager;

	sf::Vector2f velocity;
	float speed;	//The initial movement speed assigned when the player begins moving in a new direction

	sf::Uint16 playerPort;	//The UDP port to which the respective client is bound, used as a type of ID
};