//@BridgetACasey

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void handleEvents();

	void beginRender();
	void endRender();
	void render(sf::Drawable& object);

	inline sf::RenderWindow* getWindow() const { return window; }
	const inline sf::Vector2f& getResolutionScale() const { return resolutionScale; }
	 
private:
	const void setResolutionScale(unsigned int width, unsigned int height);
	const void setResolution(unsigned int width, unsigned int height);
	
	sf::RenderWindow* window;

	sf::Vector2u resolution;
	sf::Vector2f resolutionScale;
};