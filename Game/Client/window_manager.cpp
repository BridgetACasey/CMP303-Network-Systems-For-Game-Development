//@BridgetACasey

#include "window_manager.h"

WindowManager::WindowManager()
{
	window = new sf::Window(sf::VideoMode(800, 600), "Client");
}

WindowManager::~WindowManager()
{
	if (window)
	{
		delete window;
	}
}

void WindowManager::run()
{
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }
    }
}