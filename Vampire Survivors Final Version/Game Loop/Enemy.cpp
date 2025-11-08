#include "GamesEngineeringBase.h"
#include <random>
#include <algorithm> 
#include <iostream>
#include "Enemy.h"
#include "Player.h"


// Default Constructor
Enemy::Enemy()
{
	// World co-ordinates of enemy initialised
	enemyX = 0;
	enemyY = 0;
}

// Constructor with paramters initializes the spawning in of Enemy and sets them to alive
Enemy::Enemy(float _x, float _y, GamesEngineeringBase::Window& canvas)
{
	srand(static_cast<unsigned>(time(0)));

	enemyX = _x;
	enemyY = _y;

	enemyAlive = true;

}

// Destructor
Enemy::~Enemy()
{

}

void Enemy::spawnEnemy(GamesEngineeringBase::Window& canvas)
{
	const unsigned int tileSize = 32;                                    // each tile is 32 x 32
	const unsigned int mapDimension = 80;                                // map is 80 tiles by 80 tiles
	const unsigned int worldX = mapDimension * tileSize;        // world dimensions X 
	const unsigned int worldY = mapDimension * tileSize;        // world dimensions Y

	// Spawn enemy on side of world map somewhere in their world co-ordinates
	unsigned int side = rand() % 4;    // chooses a random side to spawn

	switch (side)
	{
	case 0: // Top 
		enemyX = static_cast<float>(rand() % (worldX - enemyRightImage.width));                // random on X
		enemyY = -200;											                               // at top of world map on Y but slightly off
		break;

	case 1: // Bottom
		enemyX = static_cast<float>(rand() % (worldX - enemyRightImage.width));                // random on X
		enemyY = static_cast<float>((worldY - enemyRightImage.height) + 200);                  // at bottom of world map on Y but slightly off
		break;

	case 2: // Left
		enemyX = -200;                                                                         // left of world map on X but slightly off
		enemyY = static_cast<float>(rand() % (worldY - enemyRightImage.height));               // random on Y
		break;

	case 3: // Right
		enemyX = static_cast<float>((worldX - enemyRightImage.width) + 200);                    // right of world map on X but slightly off
		enemyY = static_cast<float>(rand() % (worldY - enemyRightImage.height));                // random on Y
		break;	
	}

	enemyAlive = true;           // ensures enemy is alive when spawning
}

void Enemy::drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas)
{
	float radiusSquared = radius * radius;
	for (float x = -radius; x < radius + 1; x++)
	{
		float y = sqrtf(radiusSquared - (x * x));

		if (centreX + x > 0 && centreX + x < canvas.getWidth())
		{
			if (centreY + y > 0 && centreY + y < canvas.getHeight())
			{
				canvas.draw(static_cast<int>(centreX + x), static_cast<int>(centreY + y), 255, 0, 0);
			}

			if (centreY - y > 0 && centreY - y < canvas.getHeight())
			{
				canvas.draw(static_cast<int>(centreX + x), static_cast<int>(centreY - y), 255, 0, 0);
			}
		}
	}
}

void Enemy::drawEnemy(GamesEngineeringBase::Window* canvas, float cameraX, float cameraY)          // draws an enemyBlueGhost
{
	// world co-ordinates to screen co-ordinates conversion
	screenX = enemyX - cameraX;
	screenY = enemyY - cameraY;

	if (enemyAlive)
	{
		GamesEngineeringBase::Image* currentDirectionImage;         

		if (lookingRight)
		{
			currentDirectionImage = &enemyRightImage;        // if looking right, use this image at this address
		}
		else
		{
			currentDirectionImage = &enemyLeftImage;         // if looking left, use this image at this address
		}

		for (int y = 0; y < currentDirectionImage->height; y++)
		{
			float drawY = screenY + y;                            // check screen y + pixel image y
			if (drawY > 0 && drawY < canvas->getHeight())		  // if can draw y, move on
			{
				for (int x = 0; x < currentDirectionImage->width; x++)
				{
					float drawX = screenX + x;                        // check screen x + pixel image x
					if (drawX > 0 && drawX < canvas->getWidth())      // if can draw x, move on
					{
						if (currentDirectionImage->alphaAt(x, y) > 0)
						{
							canvas->draw(static_cast<int>(screenX) + x, static_cast<int>(screenY) + y, currentDirectionImage->at(x, y));  // if the code makes it this far, then image can be drawn
						}
					}
				}
			}
		}
		//drawCollisionCircle(screenX + currentDirectionImage->width / 2, screenY + currentDirectionImage->height / 2, 30, *canvas);
	}
}

void Enemy::moveEnemyTowardsPlayer(Player& player)                 
{
	float dx = (player.playerX + player.cameraX) - enemyX;                      // use world co-ords of enemy and world co-ords of player
	float dy = (player.playerY + player.cameraY) - enemyY;
	float dxSquared = dx * dx;
	float dySquared = dy * dy;
	float distanceSquared = dxSquared + dySquared;
	float distanceFromPlayer = sqrt(distanceSquared);

	if (distanceFromPlayer > 10.0f)                                               // if distance is 0, the screen flickers as enemy on top of player so 10 is a good value
	{
		float NormalisedX = dx / distanceFromPlayer;
		float NormalisedY = dy / distanceFromPlayer;

		enemyX += NormalisedX * moveSpeed * moveSpeedSlowRate;
		enemyY += NormalisedY * moveSpeed * moveSpeedSlowRate;

	}

	// while enemy moves, it changes looking direction based on player's position
	if (enemyX < player.playerX + player.cameraX)
	{
		lookingRight = true;
	}
	else
	{
		lookingRight = false;
	}
}

bool Enemy::enemyCollidePlayer(Player& player)
{
	float dx = (player.playerX + player.cameraX) - enemyX;      // get world co-ords of enemy and player
	float dy = (player.playerY + player.cameraY) - enemyY;

	float radius = 50;
	float radiusSquared = radius * radius;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < radiusSquared)
	{
		player.health -= 10;
		std::cout << "Player health: " << player.health << std::endl;
		setEnemyDead();    // when enemies touch player, the player loses health and the enemy is set to dead and wil be deleted in Enemy Manager

		if (player.health <= 0)
		{
			player.Dead();
		}
		return true;

	}
	return false;
}

bool Enemy::enemyCollideBullet(Player& player)
{
	return false;
}

bool Enemy::enemyCollideAbility(Player& player)
{
	
	return false;
}

// Function which when called, sets the enemy to dead so it is not drawn
void Enemy::setEnemyDead()
{
	enemyAlive = false;
}

// Flags enemy as dead
bool Enemy::isEnemyDead() const
{
	return !enemyAlive;   // enemy Alive is false, but the not makes it true, so enemy is dead
}

float Enemy::getEnemyX()  // used when saving the game
{
	return enemyX;
}

float Enemy::getEnemyY()    // used when saving the game
{
	return enemyY;
}

void Enemy::showEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float deltaTime, float cameraX, float cameraY)
{

}

void Enemy::spawnEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float cameraX, float cameraY)
{

}

void Enemy::drawEnemyProjectile(GamesEngineeringBase::Window& canvas, float cameraX, float cameraY)
{
	
}

void Enemy::moveEnemyProjectile(GamesEngineeringBase::Window& canvas, Player& player, float& enemyProjectileX, float& enemyProjectileY)
{
	
}

bool Enemy::projectileCollidePlayer(Player& player)
{

	return false;
}

void Enemy::setProjectileDead()
{

}

float Enemy::getProjectileX()
{
	return 0.0f;
}

float Enemy::getProjectileY()
{
	return 0.0f;
}