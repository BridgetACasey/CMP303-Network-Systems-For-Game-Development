//@BridgetACasey

#include "game_object.h"
#include <cmath>

float linearLerp(float a, float b, float t) { return a + (t * (b - a)); }

GameObject::GameObject()
{
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
	//Linerally interpolate between the current position and the next predicted position
	float posX = linearLerp(getPosition().x, nextPosition.x, 0.05f);
	float posY = linearLerp(getPosition().y, nextPosition.y, 0.05f);

	setPosition(posX, posY);
}