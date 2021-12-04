//@BridgetACasey

#pragma once

#include "state.h"
#include "object/ui_button.h"

//The main menu state - allows user to transition to the game state or close the application
class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void setup() override;

	void onEnter() override;
	void onExit() override;

	bool update(float deltaTime) override;
	void render() override;

private:
	UIButton* connectButton;
	UIButton* quitButton;
};