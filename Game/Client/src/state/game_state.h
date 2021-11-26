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
	void createNewPlayerInstance();
	void updatePlayerPositions(float deltaTime);
	void updateChatLog(float deltaTime);
	
	Player* player;

	std::vector<GameObject*> otherPlayers;
	
	GameObject* chatBar;

	ChatManager* chatManager;

	UIButton* chatButton;
	UIButton* playButton;

	bool playing;
};