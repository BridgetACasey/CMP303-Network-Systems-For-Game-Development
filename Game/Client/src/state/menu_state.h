//@BridgetACasey

#pragma once

#include "state.h"
#include "object/ui_button.h"
#include "SFML/Graphics/Texture.hpp"

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
	UIButton* connectButton;
	UIButton* quitButton;
};