//@BridgetACasey

#pragma once

#include "window_manager.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	float getDeltaTime();

	inline WindowManager* getWindowManager() const { return windowManager; }

private:
	WindowManager* windowManager;
};