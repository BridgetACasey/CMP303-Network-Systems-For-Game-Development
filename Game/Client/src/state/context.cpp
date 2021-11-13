//@BridgetACasey

#include "context.h"

Context::Context()
{
	activeState = nullptr;

	input = new InputManager();

	MenuState* menu = new MenuState();
	GameState* game = new GameState();

	states[StateLabel::MENU] = menu;
	states[StateLabel::GAME] = game;
}

Context::~Context()
{
	delete states[StateLabel::MENU];
	delete states[StateLabel::GAME];
}

void Context::setActiveState(StateLabel stateName)
{
	//Changes the active state and passes a reference to the same instance of the context class
	if (activeState)
	{
		activeState->onExit();
	}

	activeState = states[stateName];

	activeState->setContext(this);

	activeState->onEnter();
}