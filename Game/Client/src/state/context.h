//@BridgetACasey

#pragma once

#include <map>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "input/input_manager.h"
#include "game/window_manager.h"
#include "menu_state.h"
#include "game_state.h"

class Context
{
public:
	Context(WindowManager* window, sf::TcpSocket* tcp, sf::UdpSocket* udp);
	~Context();

	void setActiveState(StateLabel stateName);

	inline State* getActiveState() const { return activeState; }

	inline InputManager* getInputManager() const { return inputManager; }
	inline WindowManager* getWindowManager() const { return windowManager; }

	inline sf::TcpSocket* getTCP() const { return socketTCP; }
	inline sf::UdpSocket* getUDP() const { return socketUDP; }

private:
	State* activeState;

	std::map<StateLabel, State*> states;

	InputManager* inputManager;

	WindowManager* windowManager;

	sf::TcpSocket* socketTCP;
	sf::UdpSocket* socketUDP;
};