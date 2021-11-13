//@BridgetACasey

#pragma once

#include "game_object.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	const void update(float deltaTime) override;
};