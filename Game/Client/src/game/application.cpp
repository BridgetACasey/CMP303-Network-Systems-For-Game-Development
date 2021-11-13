//@BridgetACasey

#include "application.h"

#include <SFML\System\Clock.hpp>

Application::Application()
{
	windowManager = new WindowManager();
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
	windowManager->run();
}

float Application::getDeltaTime()
{
	sf::Clock gameClock;

	return  gameClock.restart().asSeconds();
}