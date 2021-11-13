//@BridgetACasey

#pragma once

#include "window_manager.h"
#include "state/context.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	float getDeltaTime() const;

	inline WindowManager* getWindowManager() const { return windowManager; }

private:
	WindowManager* windowManager;

	Context* context;
};