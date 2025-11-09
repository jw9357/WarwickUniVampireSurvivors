#pragma once

#include "GamesEngineeringBase.h"
#include <random>
#include <algorithm> 
#include <iostream>
#include "Player.h"

class Player;   // forward declare Player

class Shooter : public Enemy
{
private:

protected:
	int health;

public:
	bool enemyAlive = true;
	bool lookingRight = true;

	GamesEngineeringBase::Image enemyProjectileImage;

	float enemyProjectileX;
	float enemyProjectileY;
	float projectileMoveSpeed = 15.0f;
	float drawProjectileTimer;
	float projectileSpeed;
	bool enemyProjectileActive;
	

	// Default Constructor
	Shooter();

	// Constructor with paramters initializes the spawning in of RedGhost and sets them to alive
	Shooter(float _X, float _Y, GamesEngineeringBase::Window& canvas);

	// Destructor
	~Shooter();

	void moveEnemyTowardsPlayer(Player& player) override;

	bool enemyCollideBullet(Player& player) override;

	bool enemyCollideAbility(Player& player) override;

	void showEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float deltaTime, float cameraX, float cameraY) override;

	void spawnEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float cameraX, float cameraY) override;

	void drawEnemyProjectile(GamesEngineeringBase::Window& canvas, float cameraX, float cameraY) override;

	void moveEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float& enemyProjectileX, float& enemyProjectileY) override;

	bool projectileCollidePlayer(Player& player) override;

	void setProjectileDead() override;

	float getProjectileX() override;

	float getProjectileY() override;
};