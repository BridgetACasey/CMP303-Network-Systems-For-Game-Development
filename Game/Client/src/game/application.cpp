//@BridgetACasey

#include "application.h"

#include <SFML/Window/Event.hpp>

Application::Application()
{
	networkManager = new NetworkManager();
	windowManager = new WindowManager();

	context = new Context(networkManager, windowManager);
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

	//Run the client application window and check for events
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
				if(event.key.code > -1)	//Check that the key pressed is valid, otherwise don't pass it to InputManager
				context->getInputManager()->setKeyStatus(event.key.code, InputStatus::PRESSED);
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code > -1) //Check that the key released is valid, otherwise don't pass it to InputManager
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
					//Grab the last printable key that was pressed and convert to char
					context->getInputManager()->setCurrentChar((char)event.text.unicode);
				}
			}

			else
			{
				context->getInputManager()->setCurrentChar(0);	//No printable keys are being pressed, reset the current char
			}
		}

		//So long as the active state is being updated, keep the SFML window open and render to it
		running = context->getActiveState()->update(getDeltaTime());
		context->getActiveState()->render();
	}
}