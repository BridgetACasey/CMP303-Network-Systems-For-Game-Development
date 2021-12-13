//@BridgetACasey

#pragma once

#include "state.h"
#include "object/player.h"
#include "input/chat_manager.h"

//The playable game state - contains player objects and allows for chatting
class GameState : public State
{
public:
	GameState();
	~GameState();

	void setup() override;

	void onEnter() override;
	void onExit() override;

	bool update(float deltaTime) override;
	void render() override;

private:
	inline int getClientTime() const { return clientClock.getElapsedTime().asMilliseconds(); }
	
	void updateGameState(float deltaTime);
	void checkQuit();
	void updateUI();
	void updatePlayerCount(sf::Packet& receivedPacket);
	void createPlayerInstance(sf::Uint16 port);
	void removePlayerInstance(sf::Uint16 port);
	void updatePlayerPositions(float deltaTime);

	void updateChatLog(sf::Packet& receivedPacket, float deltaTime);
	
	bool sendUpdate(float period);

	float elapsedTime;	//The total elapsed time in ms since connecting to the server and entering the game state
	float lastTime;	//The time at which a packet was last sent to the server

	sf::Clock clientClock;

	Player* player;

	std::vector<Player*> otherPlayers;
	
	GameObject* chatBar;

	ChatManager* chatManager;

	UIButton* chatButton;
	UIButton* playButton;

	sf::Font arial;
	sf::Text diagnosticText;
	sf::Text ghostText;
	sf::Text moveText;
	sf::Text modeText;

	bool playing;	//Used to determine if the user is actively controlling their avatar or chatting
	bool running;

	bool enableGhosts;

	float tickRate;	//The variable rate at which updates are being sent to the server
	float latency;
};