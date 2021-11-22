//@BridgetACasey

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "state.h"
#include "object/player.h"
#include "input/chat_manager.h"
#include "network/player_data.h"
#include "network/chat_data.h"

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
	void sendPlayerData(PlayerData& playerData);
	void sendChatData(ChatData& chatData);

	Player* player;
	
	ChatManager* chatManager;

	UIButton* chatButton;
	UIButton* playButton;

	sf::Font arial;
	sf::Text chatText;

	bool playing;
};