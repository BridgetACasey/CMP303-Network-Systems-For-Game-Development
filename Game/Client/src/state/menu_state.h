//@BridgetACasey

#pragma once

#include "state.h"
#include "object/ui_button.h"

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