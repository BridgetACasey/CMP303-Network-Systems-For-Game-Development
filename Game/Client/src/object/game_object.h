//@BridgetACasey

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

//A basic game object based on an SFML rectangle object
class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	~GameObject();

	const virtual void update(float deltaTime);
	const void interpolate(float deltaTime);

	const inline void setNextPosition(float px, float py) { nextPosition.x = px; nextPosition.y = py; }
	const inline sf::Vector2f& getNextPosition() const { return nextPosition; }

	const inline void setVelocity(float vx, float vy) { velocity.x = vx; velocity.y = vy; }
	const inline sf::Vector2f& getVelocity() const { return velocity; }

protected:
	sf::Vector2f nextPosition;	//The predicted next position, used when the object is not user-controllable
	sf::Vector2f velocity;
};