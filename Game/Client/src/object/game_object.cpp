//@BridgetACasey

#include "game_object.h"

GameObject::GameObject()
{
	collisionBox.height = 1.0f;
	collisionBox.width = 1.0f;

	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

GameObject::~GameObject()
{

}

const void GameObject::update(float deltaTime)
{

}

const void GameObject::setObjectSize(float width, float height)
{
	setSize(sf::Vector2f(width, height));

	collisionBox.width = width;
	collisionBox.height = height;
}