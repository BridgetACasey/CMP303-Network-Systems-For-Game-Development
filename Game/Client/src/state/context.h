//@BridgetACasey

#pragma once

#include <map>

#include "input/input_manager.h"
#include "game/window_manager.h"
#include "network/network_manager.h"
#include "menu_state.h"
#include "game_state.h"

class Context
{
public:
	Context(NetworkManager* network, WindowManager* window);
	~Context();

	void setActiveState(StateLabel stateName);

	inline State* getActiveState() const { return activeState; }

	inline InputManager* getInputManager() const { return inputManager; }
	inline NetworkManager* getNetworkManager() const { return networkManager; }
	inline WindowManager* getWindowManager() const { return windowManager; }

private:
	State* activeState;

	std::map<StateLabel, State*> states;

	InputManager* inputManager;
	NetworkManager* networkManager;
	WindowManager* windowManager;

	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;
};