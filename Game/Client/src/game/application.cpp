//@BridgetACasey

#include "application.h"

#include <SFML\System\Clock.hpp>

Application::Application()
{
	windowManager = new WindowManager();
	context = new Context();
}

Application::~Application()
{
	if (windowManager)
	{
		delete windowManager;
	}
}

void Application::run()
{
	//windowManager->run();
	while (windowManager->getWindow()->isOpen())
	{
		windowManager->handleEvents();

		context->getActiveState()->handleInput();
		context->getActiveState()->update(0.0f);
		context->getActiveState()->render();
	}
}

float Application::getDeltaTime() const
{
	sf::Clock gameClock;

	return  gameClock.restart().asSeconds();
}