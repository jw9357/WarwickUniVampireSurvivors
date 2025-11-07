#pragma once

#include "GamesEngineeringBase.h"
#include <iostream>
#include <algorithm> 
#include "Enemy.h"

class Enemy;  // forward declare Enemy

class PowerUp;   // forward declare PowerUp

class Player
{
private:

public:
	float playerX;  
	float playerY;  

	float cameraX;
	float cameraY;

	int health = 500;
	int score = 0;
	bool playerAlive = true;
	bool lookingRight = true;
	GamesEngineeringBase::Image playerRightImage;     // Set up image of player looking right
	GamesEngineeringBase::Image playerLeftImage;      // Set up image of player looking left

	float bulletX;
	float bulletY;
	int bulletSpeed = 8;
	bool bulletActive = false;
	bool bulletRight = true;
	GamesEngineeringBase::Image bulletImage;          // Set up image of bullet

	float abilityX;
	float abilityY;
	bool drawSpecialAbilityActive = false;
	GamesEngineeringBase::Image abilityImage;          // Set up image of ability
	bool abilityReady = true;
	float cooldownShowingAbility = 2.0f;
	float cooldownTilNextButtonPress;
	bool resetActive = false;
	
	// Constructor with paramters initializes the position of the player in the middle of the screen
	Player(GamesEngineeringBase::Window& canvas);
	
	void drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas);

	void drawPlayer(GamesEngineeringBase::Window* canvas, float cameraX, float cameraY);

	void lookLeft();
	
	void lookRight();

	void drawBullet(float bulletX, float bulletY, GamesEngineeringBase::Window& canvas);

	void shoot();

	void moveBullet(GamesEngineeringBase::Window& canvas);

	void activateAbility();

	void drawAbility(float abilityX, float abilityY, GamesEngineeringBase::Window& canvas, float deltaTime);

	void startAbilityDrawCooldown(float deltaTime, const PowerUp& powerUp);

	void startAbilityResetCooldown(float deltaTime);

	void Dead();

	bool isAlive() const;

	void setScore(int _score);

	void setHealth(int _health);

	void setPosition(float _playerX, float _cameraX, float _playerY, float _cameraY);

};