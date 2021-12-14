//@BridgetACasey

#pragma once

#include "game_object.h"
#include <string>
#include <SFML/Graphics/Text.hpp>

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

	void setPlayerTexture(const std::string& filePath);

	void createGhost(const std::string& filePath);

	inline void setNamePlate(sf::Text* name) { namePlate = name; }

	inline sf::Text* getNamePlate() const { return namePlate; }

	inline GameObject* getGhost() const { return ghost; }

	inline sf::Vector2f& getVelocity() { return velocity; }

	inline void setPlayerPort(sf::Uint16 port) { playerPort = port; }
	inline sf::Uint16 getPlayerPort() const { return playerPort; }

	inline void setActivePlayer(bool active) { activePlayer = active; }
	inline bool getActivePlayer() const { return activePlayer; }

	inline void setConstantMove(bool move) { constantMove = move; }
	inline bool getConstantMove() const { return constantMove; }

	inline void setElapsedTime(float time) { elapsedTime = time; }
	inline void setLastUpdateTime(float time) { lastUpdateTime = time; }
	
	inline bool getTimeout() const { return timeout; }

private:
	InputManager* inputManager;

	sf::Vector2f velocity;
	float speed;	//The initial movement speed assigned when the player begins moving in a new direction, used to calculate velocity

	sf::Uint16 playerPort;	//The UDP port to which the respective client is bound, used as a type of ID

	GameObject* ghost;	//Object rendered at the predicted next position, only appears for non-active player instances

	sf::Text* namePlate;

	bool activePlayer;

	bool constantMove;	//To check if the player should stop moving once the WASD keys are released, or continue in the same direction

	float elapsedTime;	//The total elapsed time in ms since this player connected to the server
	float lastUpdateTime;	//The time at which a packet was last received from the server for this specific player

	bool timeout;
};