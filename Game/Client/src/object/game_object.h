//@BridgetACasey

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	~GameObject();

	const virtual void update(float deltaTime);

	const void setObjectSize(float width, float height);

	const inline void setVelocity(float vx, float vy) { velocity.x = vx; velocity.y = vy; }
	const inline sf::Vector2f& getVelocity() const { return velocity; }

protected:
	sf::FloatRect collisionBox;

	sf::Vector2f velocity;
};