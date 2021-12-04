//@BridgetACasey

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

//Encapsulates data and functionality relevant to the SFML render window object
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void beginRender();
	void endRender();
	void render(sf::Drawable& object);

	const void setResolutionScale(unsigned int width, unsigned int height);
	const void setResolution(unsigned int width, unsigned int height);

	inline sf::RenderWindow* getWindow() const { return window; }
	const inline sf::Vector2f& getResolutionScale() const { return resolutionScale; }
	 
private:	
	sf::RenderWindow* window;

	sf::Vector2u resolution;
	sf::Vector2f resolutionScale;	//A multiplier for determining where to place objects on the screen relative to the new resolution
};