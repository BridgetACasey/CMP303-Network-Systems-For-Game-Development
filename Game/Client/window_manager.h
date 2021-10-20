//@BridgetACasey

#pragma once

#include <SFML/Window.hpp>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void run();

	inline sf::Window* getWindow() const { return window; }

private:
	sf::Window* window;
};