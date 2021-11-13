//@BridgetACasey

#pragma once

class Context;

enum class StateLabel	//For switching between states in a more readable way
{
	MENU = 0,
	GAME
};

//A purely virtual class that all states inherit from
class State
{
protected:
	State(){ context = nullptr; }

public:
	virtual ~State() = default;

	virtual void setup() = 0;

	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	virtual void handleInput() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual void render() = 0;

	void setContext(Context* cntxt) { this->context = cntxt; }

protected:
	Context* context;

	/*
	* Used to determine if objects need to be created.
	* They cannot be created in the state's constructor, as many objects rely on a reference to context,
	* which is only passed to the class upon it becoming the active state.
	*/
	bool firstSetup = true;
};