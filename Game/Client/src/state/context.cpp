//@BridgetACasey

#include "context.h"

Context::Context(NetworkManager* network, WindowManager* window)
{
	activeState = nullptr;

	networkManager = network;
	windowManager = window;
	inputManager = new InputManager(window);

	MenuState* menu = new MenuState();
	GameState* game = new GameState();

	states[StateLabel::MENU] = menu;
	states[StateLabel::GAME] = game;

	setActiveState(StateLabel::MENU);
}

Context::~Context()
{
	if (states[StateLabel::MENU])
	{
		delete states[StateLabel::MENU];
	}

	if (states[StateLabel::GAME])
	{
		delete states[StateLabel::GAME];
	}
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