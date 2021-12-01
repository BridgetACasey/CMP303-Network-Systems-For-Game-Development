//@BridgetACasey

#pragma once

#include "state.h"
#include "object/player.h"
#include "input/chat_manager.h"

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
	
	void updatePlayerCount();
	void createPlayerInstance(int id);
	void removePlayerInstance(int id);
	void updatePlayerPositions(float deltaTime);

	void updateChatLog(float deltaTime);
	
	bool sendUpdate(float period);

	float elapsedTime;
	float lastTime;

	sf::Clock clientClock;

	Player* player;

	std::vector<Player*> otherPlayers;
	
	GameObject* chatBar;

	ChatManager* chatManager;

	UIButton* chatButton;
	UIButton* playButton;

	bool playing;
	bool running;
};