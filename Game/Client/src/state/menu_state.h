//@BridgetACasey

#pragma once

#include "state.h"

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void setup() override;

	void onEnter() override;
	void onExit() override;
	
	void handleInput() override;
	bool update(float deltaTime) override;
	void render() override;

private:
};