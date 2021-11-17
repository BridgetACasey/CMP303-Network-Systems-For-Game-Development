//@BridgetACasey

#include "context.h"
#include "menu_state.h"

MenuState::MenuState()
{
	connectButton = nullptr;
	quitButton = nullptr;
}

MenuState::~MenuState()
{

}

void MenuState::setup()
{
	connectButton = new UIButton(context->getInputManager(), "assets/potatolizard.png", "assets/wonky-jaw.png");
	connectButton->setPosition(sf::Vector2f(550.0f, 300.0f));
	connectButton->setSize(sf::Vector2f(125.0f, 125.0f));

	quitButton = new UIButton(context->getInputManager(), "assets/potatolizard.png", "assets/wonky-jaw.png");
	quitButton->setPosition(sf::Vector2f(550.0f, 450.0f));
	quitButton->setSize(sf::Vector2f(125.0f, 125.0f));
}

void MenuState::onEnter()
{
	if(firstSetup)
	{
		setup();
	}
}

void MenuState::onExit()
{

}

void MenuState::handleInput()
{

}

bool MenuState::update(float deltaTime)
{
	context->getInputManager()->update(deltaTime);

	if (connectButton->isClicked())
	{
		context->setActiveState(StateLabel::GAME);
	}

	if (quitButton->isClicked())
	{
		return false;
	}

	return true;
}

void MenuState::render()
{
	context->getWindowManager()->beginRender();

	context->getWindowManager()->render(*connectButton);
	context->getWindowManager()->render(*quitButton);

	context->getWindowManager()->endRender();
}