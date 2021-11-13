//@BridgetACasey

#include "context.h"

Context::Context()
{
	activeState = nullptr;
}

Context::~Context()
{

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