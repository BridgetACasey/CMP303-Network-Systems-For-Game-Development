//@BridgetACasey

#pragma once

#include "state.h"

class GameState : public State
{
public:
	GameState();
	~GameState();

	void setup() override;

	void onEnter() override;
	void onExit() override;

	void handleInput() override;
	bool update(float deltaTime) override;
	void render() override;

private:
};