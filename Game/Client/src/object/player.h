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

private:
	InputManager* inputManager;

	sf::Vector2f velocity;
	float speed;	//The initial movement speed assigned when the player begins moving in a new direction

	sf::Uint16 playerPort;	//The UDP port to which the respective client is bound, used as a type of ID

	GameObject* ghost;

	sf::Text* namePlate;

	bool activePlayer;

	bool constantMove;
};