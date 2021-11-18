//@BridgetACasey

#pragma once

#include <SFML/System/Clock.hpp>

#include "window_manager.h"
#include "state/context.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	inline float getDeltaTime() { return gameClock.restart().asSeconds(); }

	inline WindowManager* getWindowManager() const { return windowManager; }

private:
	sf::Clock gameClock;
	
	WindowManager* windowManager;

	Context* context;
};