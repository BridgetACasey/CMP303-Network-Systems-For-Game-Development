//@BridgetACasey

#include "application.h"

#include <SFML/Window/Event.hpp>

#define SERVERIP "127.0.0.1"

#define SERVERPORT 5555

Application::Application()
{
	windowManager = new WindowManager();
	context = new Context(windowManager, &socketTCP, &socketUDP);
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
	bool running = true;

	socketTCP.connect(SERVERIP, SERVERPORT);
	//socketUDP.bind(SERVERPORT, SERVERIP);

	while (running)
	{
		sf::Event event;

		while (windowManager->getWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				windowManager->getWindow()->close();
				running = false;
			}

			if (event.type == sf::Event::Resized)
			{
				windowManager->setResolutionScale(event.size.width, event.size.height);
				windowManager->setResolution(event.size.width, event.size.height);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				context->getInputManager()->setKeyStatus(event.key.code, InputStatus::PRESSED);
			}

			if (event.type == sf::Event::KeyReleased)
			{
				context->getInputManager()->setKeyStatus(event.key.code, InputStatus::RELEASED);
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

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode > 31 && event.text.unicode < 127)	//Can be printed to the screen
				{
					context->getInputManager()->setCurrentChar((char)event.text.unicode);
				}
			}

			else
			{
				context->getInputManager()->setCurrentChar(0);
			}
		}

		running = context->getActiveState()->update(getDeltaTime());
		context->getActiveState()->render();
	}
}