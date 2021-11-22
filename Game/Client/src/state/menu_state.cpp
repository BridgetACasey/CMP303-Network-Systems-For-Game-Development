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
	connectButton = new UIButton(context->getInputManager(), "assets/connect-active.png", "assets/connect-idle.png");
	connectButton->setPosition(sf::Vector2f(550.0f, 200.0f));
	connectButton->setSize(sf::Vector2f(125.0f, 125.0f));

	quitButton = new UIButton(context->getInputManager(), "assets/quit-active.png", "assets/quit-idle.png");
	quitButton->setPosition(sf::Vector2f(550.0f, 400.0f));
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

bool MenuState::update(float deltaTime)
{
	context->getInputManager()->update(deltaTime);

	if ((context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Escape) == InputStatus::PRESSED) || quitButton->isClicked())
	{
		context->getNetworkManager()->requestDisconnection();
		
		return false;
	}

	if (connectButton->isClicked())
	{
		context->getNetworkManager()->requestConnection();
		
		context->setActiveState(StateLabel::GAME);
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