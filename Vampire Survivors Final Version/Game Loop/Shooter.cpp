#include "GamesEngineeringBase.h"
#include <random>
#include <algorithm> 
#include <iostream>
#include "Player.h"
#include "Shooter.h"
using namespace std;


// Default Constructor
Shooter::Shooter()
{

}

// Constructor with paramters initializes the spawning in of Red Ghost and sets them to alive
Shooter::Shooter(float _x, float _y, GamesEngineeringBase::Window& canvas)
{
	// Load in image of enemyShooter
	enemyRightImage.load("Resources/EnemyShootRight.png");
	enemyLeftImage.load("Resources/EnemyShootLeft.png");
	enemyProjectileImage.load("Resources/BlueBomb.png");

	spawnEnemy(canvas);
	enemyAlive = true;

	moveSpeed = 12.0f;    // static enemy shooter

	health = 300.0f;     // Enemy with top health

	drawProjectileTimer = 20.0f;
	enemyProjectileActive = false;
	projectileSpeed = 40.0f;     // bombs can not be destroyed by player bullets so they move slowly
}

// Destructor
Shooter::~Shooter()
{

}


void Shooter::moveEnemyTowardsPlayer(Player& player)                 // Needs player's co-ordinates
{
	float moveSpeedSlowRate = 0.1f;							                    // slows the enemy considerably, adjustable value
	float dx = (player.playerX + player.cameraX) - enemyX;                      // use world co-ords of enemy and world co-ords of player
	float dy = (player.playerY + player.cameraY) - enemyY;
	float dxSquared = dx * dx;
	float dySquared = dy * dy;
	float distanceSquared = dxSquared + dySquared;
	float distanceFromPlayer = sqrt(distanceSquared);

	if (distanceFromPlayer > 300.0f)                                         
	{
		float NormalisedX = dx / distanceFromPlayer;
		float NormalisedY = dy / distanceFromPlayer;

		enemyX += NormalisedX * moveSpeed * moveSpeedSlowRate;
		enemyY += NormalisedY * moveSpeed * moveSpeedSlowRate;

	}
}

bool Shooter::enemyCollideBullet(Player& player)
{
	float dx = (player.bulletX + player.cameraX) - enemyX;                      // use world co-ords of enemy and world co-ords of player
	float dy = (player.bulletY + player.cameraY) - enemyY;

	float radius = 40.0f;
	float radiusSquared = radius * radius;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < radiusSquared)
	{
		if (!player.bulletActive)                // Prevents bullet from constantly hitting the enemy
		{
			return false;
		}

		const unsigned int damage = 50;
		health -= damage;
		//std::cout << "Enemy Hit, health is now " << health << std::endl;

		player.bulletActive = false;             // bullet is not drawn but still exists in memory, memory leak bug

		if (health <= 0)
		{
			//std::cout << "Enemy Dead" << std::endl;
			player.score += 300;
			std::cout << "Current Player score: " << player.score << std::endl;

			return true;
		}
	}
	return false;
}



bool Shooter::enemyCollideAbility(Player& player)
{
	if (player.drawSpecialAbilityActive && player.abilityReady)         // if the player's special ability is activated
	{
		std::cout << "Enemy Hit" << std::endl;
		player.score += 100;
		std::cout << player.score << std::endl;

		return true;
	}

	return false;
}


void Shooter::showEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float deltaTime, float cameraX, float cameraY)
{
	if (!enemyProjectileActive)                    // if the projectile is not active
	{
		drawProjectileTimer -= deltaTime;          // start countdown timer once enemy is spawned in

		if (drawProjectileTimer <= 0.0f)           // if timer reaches zero and enemyProjectileActive is false
		{
			spawnEnemyProjectile(canvas, player, cameraX, cameraY);          // spawn projectile on screen
			drawProjectileTimer = 0.0f;            // projectile timer stops, enemy only throw one projectile at a time until that projectile disappears
			enemyProjectileActive = true;          // enemyProjectileActive is set to true
		}
	}
	
	if (enemyProjectileActive) 
	{
		drawEnemyProjectile(canvas, cameraX, cameraY);              // if projectile spawned and active, then draw it
		moveEnemyProjectile(canvas, player, enemyProjectileX, enemyProjectileY);  // and then move it
	}
}

void Shooter::spawnEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float cameraX, float cameraY)
{	
	// where the projectile spawns in, it is near the enemy who is firing it
	enemyProjectileX = enemyX + 50;
	enemyProjectileY = enemyY;
}



void Shooter::drawEnemyProjectile(GamesEngineeringBase::Window& canvas, float cameraX, float cameraY)
{
	if (enemyProjectileActive)
	{
		// converts world enemy projectile position to screen enemy projectile position for drawing purposes
		screenX = enemyProjectileX - cameraX;
		screenY = enemyProjectileY - cameraY;

		drawCollisionCircle(screenX + enemyProjectileImage.width / 2, screenY + enemyProjectileImage.height / 2, 20, canvas);

		for (int y = 0; y < enemyProjectileImage.height; y++)      // draws every y pixel and every x pixel of image on screen
		{
			float drawY = screenY + y;                          
			if (drawY > 0 && drawY < canvas.getHeight())
			{
				for (int x = 0; x < enemyProjectileImage.width; x++)
				{
					float drawX = screenX + x;                       
					if (drawX > 0 && drawX < canvas.getWidth())
					{
						if (enemyProjectileImage.alphaAt(x, y) > 0)
						{
							canvas.draw(static_cast<int>(screenX) + x, static_cast<int>(screenY) + y, enemyProjectileImage.at(x, y));
						}
					}
				}
			}
		}
	}
}

void Shooter::moveEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float& enemyProjectileX, float& enemyProjectileY)
{
	float moveSpeedSlowRate = 0.1f;                                          // easy way to adjust projectile speed
	float dx = (player.playerX + player.cameraX) - enemyProjectileX;         // move projectile towards Player's position
	float dy = (player.playerY + player.cameraY) - enemyProjectileY;
	float dxSquared = dx * dx;
	float dySquared = dy * dy;
	float distanceSquared = dxSquared + dySquared;  
	float distanceFromPlayer = sqrt(distanceSquared);
	
	if (distanceFromPlayer > 10.0f)                                               // if distance is 0, the screen flickers as projectile on top of player so 10 is a good value
	{
		float NormalisedX = dx / distanceFromPlayer;
		float NormalisedY = dy / distanceFromPlayer;

		enemyProjectileX += NormalisedX * projectileMoveSpeed * moveSpeedSlowRate;
		enemyProjectileY += NormalisedY * projectileMoveSpeed * moveSpeedSlowRate;
	}
}

bool Shooter::projectileCollidePlayer(Player& player)
{
	float dx = (player.playerX + player.cameraX) - enemyProjectileX;         // move projectile towards Player's position
	float dy = (player.playerY + player.cameraY) - enemyProjectileY;

	float radius = 10;
	float radiusSquared = radius * radius;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < radiusSquared)
	{
		player.health -= 10;
		std::cout << "Player health: " << player.health << std::endl;
		setProjectileDead();    // when projectile touches player, the player loses health and the projectile is set to dead

		if (player.health <= 0)
		{
			player.Dead();
		}
		return true;

	}
	return false;
}

// Function which when called, sets the enemy to dead so it is not drawn
void Shooter::setProjectileDead()
{
	enemyProjectileActive = false;
}


float Shooter::getProjectileX()
{
	return enemyProjectileX;
}

float Shooter::getProjectileY()
{
	return enemyProjectileY;
}