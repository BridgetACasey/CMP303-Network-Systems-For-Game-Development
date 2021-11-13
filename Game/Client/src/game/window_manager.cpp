//@BridgetACasey

#include <SFML/Window/Event.hpp>

#include "window_manager.h"

WindowManager::WindowManager()
{
    windowSize.x = 800;
    windowSize.y = 600;

	window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Client");
}

WindowManager::~WindowManager()
{
	if (window)
	{
		delete window;
	}
}

void WindowManager::handleEvents()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }

        if (event.type == sf::Event::Resized)
        {
            window->setSize(window->getSize());
        }
    }
}