//@BridgetACasey

#include "context.h"
#include "menu_state.h"

MenuState::MenuState()
{
	connectButton = nullptr;
	quitButton = nullptr;
	chat = nullptr;
	addressText = nullptr;
}

MenuState::~MenuState()
{
	if (connectButton)
	{
		delete connectButton;
	}

	if (quitButton)
	{
		delete quitButton;
	}

	if (chat)
	{
		delete chat;
	}

	if (addressText)
	{
		delete addressText;
	}
}

void MenuState::setup()
{
	connectButton = new UIButton(context->getInputManager(), "assets/connect-active.png", "assets/connect-idle.png");
	connectButton->setPosition(sf::Vector2f(550.0f, 250.0f));
	connectButton->setSize(sf::Vector2f(125.0f, 125.0f));

	quitButton = new UIButton(context->getInputManager(), "assets/quit-active.png", "assets/quit-idle.png");
	quitButton->setPosition(sf::Vector2f(550.0f, 450.0f));
	quitButton->setSize(sf::Vector2f(125.0f, 125.0f));

	addressText = new sf::Text();

	addressText->setPosition(sf::Vector2f(475.0f, 125.0f));
	addressText->setCharacterSize(42);
	addressText->setFillColor(sf::Color::White);
	addressText->setStyle(sf::Text::Style::Bold);
	addressText->setOutlineThickness(2.5f);
	addressText->setOutlineColor(sf::Color::Black);

	chat = new ChatManager(context->getInputManager(), addressText, 12);

	chat->setInputMessage(sf::String("192.168.0.18"));
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
	//Read input from the user
	chat->updateMessageStream(deltaTime);
	
	if (connectButton->isClicked())
	{
		//Grab input text and convert to SFML ip address format, then attempt to connect to server
		if (context->getNetworkManager()->requestConnection(chat->getInputText()->getString()))
		{
			context->setActiveState(StateLabel::GAME);
		}
	}

	if ((context->getInputManager()->getKeyStatus(sf::Keyboard::Key::Escape) == InputStatus::PRESSED) || quitButton->isClicked())
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

	context->getWindowManager()->render(*chat->getInputText());

	context->getWindowManager()->endRender();
}