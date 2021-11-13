//@BridgetACasey

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void handleEvents();

	inline sf::RenderWindow* getWindow() const { return window; }

private:
	sf::RenderWindow* window;

	sf::Vector2i windowSize;
};