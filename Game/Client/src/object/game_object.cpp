//@BridgetACasey

#include "game_object.h"
#include <cmath>

float lerp(float a, float b, float t) { return a + (t * (b - a)); }

GameObject::GameObject()
{
	collisionBox.height = 1.0f;
	collisionBox.width = 1.0f;

	nextPosition.x = 0.0f;
	nextPosition.y = 0.0f;

	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

GameObject::~GameObject()
{

}

const void GameObject::update(float deltaTime)
{
	interpolate(deltaTime);
}

const void GameObject::interpolate(float deltaTime)
{
	float posX = lerp(getPosition().x, nextPosition.x, 0.05f);
	float posY = lerp(getPosition().y, nextPosition.y, 0.05f);

	setPosition(posX, posY);
}

const void GameObject::setObjectSize(float width, float height)
{
	setSize(sf::Vector2f(width, height));

	collisionBox.width = width;
	collisionBox.height = height;
}