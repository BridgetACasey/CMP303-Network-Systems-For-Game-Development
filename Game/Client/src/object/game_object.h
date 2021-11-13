//@BridgetACasey

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	const virtual void update(float deltaTime);

	const void setSize(float width, float height);
	const inline sf::Vector2f& getSize() const { return sprite->getSize(); }

	const inline void setPosition(float x, float y) { sprite->setPosition(sf::Vector2f(x, y)); }
	const inline sf::Vector2f& getPosition() const { return sprite->getPosition(); }

	const inline void setVelocity(float vx, float vy) { velocity.x = vx; velocity.y = vy; }
	const inline sf::Vector2f& getVelocity() const { return velocity; }

	const inline void setSpriteTexture(sf::Texture& texture) { sprite->setTexture(&texture); }
	const inline sf::Texture& getSpriteTexture() const { return *sprite->getTexture(); }

private:
	sf::RectangleShape* sprite;
	sf::FloatRect collisionBox;

	sf::Vector2f velocity;
};