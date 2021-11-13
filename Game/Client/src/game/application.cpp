//@BridgetACasey

#include "application.h"

#include <SFML\System\Clock.hpp>

Application::Application()
{
	windowManager = new WindowManager();
	context = new Context(windowManager);
}

Application::~Application()
{
	if (context)
	{
		delete context;
	}

	if (windowManager)
	{
		delete windowManager;
	}
}

void Application::run()
{
	while (windowManager->getWindow()->isOpen())
	{
		windowManager->handleEvents();

		context->getActiveState()->handleInput();
		context->getActiveState()->update(getDeltaTime());
		context->getActiveState()->render();
	}
}

float Application::getDeltaTime() const
{
	sf::Clock gameClock;

	return  gameClock.restart().asSeconds();
}