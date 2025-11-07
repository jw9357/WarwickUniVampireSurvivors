#pragma once

#include "GamesEngineeringBase.h"
#include <iostream>
#include <algorithm> 
#include "Player.h"

class PowerUp
{
private:

public:
	float powerUpX;
	float powerUpY;

	float screenX;
	float screenY;

	GamesEngineeringBase::Image powerUpImage;     
	
	bool canDrawPowerUp;
	float drawPowerUpTimer = 60.0f;   // Once game starts, this is the time it takes for the powerUp to show in the game
	bool stopPowerUpFromAppearing;
	bool hasPowerUpBeenCollected;


	PowerUp();
	
	PowerUp(GamesEngineeringBase::Window& canvas);

	~PowerUp();

	void showPowerUp(GamesEngineeringBase::Window& canvas, float deltaTime);

	float getDrawPowerUpTimer(float drawPowerUpTimer);

	void spawnPowerUp(GamesEngineeringBase::Window& canvas);

	void drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas);

	void drawPowerUp(GamesEngineeringBase::Window* canvas, float cameraX, float cameraY);

	void powerUpCollidePlayer(Player& player);

};