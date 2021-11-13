//@BridgetACasey

#pragma once

#include <map>

#include "input/input_manager.h"

#include "menu_state.h"
#include "game_state.h"

class Context
{
public:
	Context();
	~Context();

	void setActiveState(StateLabel stateName);

	const inline State* getActiveState() const { return activeState; }

	const inline InputManager* getInputManager() const { return input; }

private:
	State* activeState;

	std::map<StateLabel, State*> states;

	InputManager* input;
};