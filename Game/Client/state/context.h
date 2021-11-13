//@BridgetACasey

#pragma once

#include <map>

#include "state.h"

class Context
{
public:
	Context();
	~Context();

	void setActiveState(StateLabel stateName);

	inline State* getActiveState() const { return activeState; }

private:
	State* activeState;

	std::map<StateLabel, State*> states;
};