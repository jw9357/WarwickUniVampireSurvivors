#pragma once

#include "GamesEngineeringBase.h"
#include <random>
#include <algorithm> 
#include <iostream>
#include "Player.h"

class Player;   // forward declare Player

class Enemy
{
private:

protected:
	float enemyX;
	float enemyY;

	float screenX;
	float screenY;

	float moveSpeed;
	float moveSpeedSlowRate = 0.1f;

	GamesEngineeringBase::Image enemyRightImage;
	GamesEngineeringBase::Image enemyLeftImage;

public:
	bool enemyAlive = true;
	bool lookingRight = true;


	// Default Constructor
	Enemy();

	// Constructor with paramters initializes the spawning in of RedGhost and sets them to alive
	Enemy(float _X, float _Y, GamesEngineeringBase::Window& canvas);

	// Destructor
	~Enemy();

	virtual void spawnEnemy(GamesEngineeringBase::Window& canvas);    // overriden in Shooter class

	void drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas);  

	void drawEnemy(GamesEngineeringBase::Window* canvas, float cameraX, float cameraY);         

	virtual void moveEnemyTowardsPlayer(Player& player);

	bool enemyCollidePlayer(Player& player);     

	virtual bool enemyCollideBullet(Player& player); 

	virtual bool enemyCollideAbility(Player& player);     // overriden in Shooter class

	void setEnemyDead();     

	bool isEnemyDead() const;

	float getEnemyX();

	float getEnemyY();

	virtual void showEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float deltaTime, float cameraX, float cameraY);  // overriden in Shooter class

	virtual void spawnEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float cameraX, float cameraY);  // overriden in Shooter class

	virtual void drawEnemyProjectile(GamesEngineeringBase::Window& canvas, float cameraX, float cameraY);   // overriden in Shooter class

	virtual void moveEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float& enemyProjectileX, float& enemyProjectileY);   // overriden in Shooter class

	virtual bool projectileCollidePlayer(Player& player);  // overriden in Shooter class

	virtual void setProjectileDead(); // overriden in Shooter class

	virtual float getProjectileX(); // overriden in Shooter class

	virtual float getProjectileY(); // overriden in Shooter class

};
