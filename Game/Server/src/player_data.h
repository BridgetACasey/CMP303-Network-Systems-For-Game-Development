//@BridgetACasey

#pragma once

struct PlayerData
{
	float time = 0.0f;
	sf::Uint16 port = 0;	//Type sf::Uint16 accounts for different implentations of unsigned short across platforms
	float posX, posY;
	float nextPosX, nextPosY;
	float velX, velY;
};