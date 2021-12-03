//@BridgetACasey

#include "window_manager.h"

WindowManager::WindowManager()
{
    resolution.x = 1200;
    resolution.y = 675;
    
    window = new sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), "Client", sf::Style::Titlebar);
}

WindowManager::~WindowManager()
{
	if (window)
	{
		delete window;
	}
}

void WindowManager::beginRender()
{
    window->clear(sf::Color(96, 96, 128, 255));
}

void WindowManager::endRender()
{
    window->display();
}

void WindowManager::render(sf::Drawable& object)
{
    window->draw(object);
}

const void WindowManager::setResolutionScale(unsigned int width, unsigned int height)
{
    resolutionScale.x = (float)width / (float)resolution.x;
    resolutionScale.y = (float)height / (float)resolution.y;
}

const void WindowManager::setResolution(unsigned int width, unsigned int height)
{
    resolution.x = width;
    resolution.y = height;

    window->setSize(resolution);
}