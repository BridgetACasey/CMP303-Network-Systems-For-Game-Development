//@BridgetACasey

#include "application.h"

#include <SFML\System\Clock.hpp>
#include <SFML/Window/Event.hpp>

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
		sf::Event event;

		while (windowManager->getWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				windowManager->getWindow()->close();
			}

			if (event.type == sf::Event::Resized)
			{
				windowManager->setResolutionScale(event.size.width, event.size.height);
				windowManager->setResolution(event.size.width, event.size.height);
			}

			if (event.type == sf::Event::MouseMoved)
			{
				context->getInputManager()->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					context->getInputManager()->setLeftMouseButton(InputStatus::PRESSED);
				}

				else if(event.mouseButton.button == sf::Mouse::Right)
				{
					context->getInputManager()->setRightMouseButton(InputStatus::PRESSED);
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					context->getInputManager()->setLeftMouseButton(InputStatus::RELEASED);
				}

				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					context->getInputManager()->setRightMouseButton(InputStatus::RELEASED);
				}
			}
		}

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