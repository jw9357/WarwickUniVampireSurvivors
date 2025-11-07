// A MANGER THAT CAN ADD MORE ENEMIES TO THE SCREEN
#include "Enemy.h"
#include "Player.h"
#include "RedGhost.h"
#include "BlueGhost.h"
#include "WhiteGhost.h"
#include "Demon.h"
#include "Shooter.h"
#include "GamesEngineeringBase.h"
#include <algorithm>

const unsigned int MAXSIZE = 500;    // 500 enemies maximum in world

class EnemyManager
{
private:
	Enemy* enemies[MAXSIZE];       // fixed array of Enemy pointers, maximum is 500
	unsigned int count;            // number of enemies currently stored
	float timeElapsed = 0.0f;      // relates to Spawn Delay
	float spawnDelay = 7.0f;       // enemies spawn in every 7 seconds but this reduces later
	int ghostNumberType;           // gets the ghost type being spawned in for saving purposes

public:
	EnemyManager()
	{
		count = 0;
	}

	void add(Enemy* enemy)                        // Add enemy types to enemy manager
	{
		if (count < MAXSIZE)
		{
			enemies[count] = enemy;
			count++;
		}
	}

	Enemy* ChooseRandomGhostToSpawn(GamesEngineeringBase::Window& canvas, int& ghostNumberType)  
	{
		unsigned int r = rand() % 5;
		ghostNumberType = r;

		switch (r)
		{
			case 0: return new BlueGhost(0, 0, canvas);
			case 1: return new WhiteGhost(0, 0, canvas);
			case 2: return new RedGhost(0, 0, canvas);
			case 3: return new Demon(0, 0, canvas);
			case 4: return new Shooter(0, 0, canvas);
			default: return new BlueGhost(0, 0, canvas);
		}
	}

	int getEnemyCount()   // getter for enemy Count, used when saving game
	{
		return count;
	}

	Enemy** getEnemiesArray()  // enemies arrays consists of enemy pointers and this returns pointers of individual enemies for saving purposes
	{
		return enemies;
	}

	int getSpawnDelay(float spawnDelay)     // getter for enemy Spawn Delay, used when saving game
	{
		return spawnDelay;
	}

	void drawAll(GamesEngineeringBase::Window& canvas, float deltaTime, float cameraX, float cameraY)          // draw all enemies in enemy manager
	{
		timeElapsed += deltaTime;                           // accumulates time since last spawn
		if (timeElapsed >= spawnDelay && count < MAXSIZE)      // if enough time has passed and the enemy manager still has enemies in it
		{
			Enemy* newEnemy = ChooseRandomGhostToSpawn(canvas, ghostNumberType);
			//std::cout << "Type of enemy spawned in is " << ghostNumberType << std::endl;
		
			enemies[count] = newEnemy;
			
			count++;
			timeElapsed -= spawnDelay;                     // slightly decrease timer or enemies all spawn at once
			spawnDelay = max(spawnDelay - 0.2f, 0.5f);
		}

		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr)      // if the space in the array is free, draw a ghost
			{
				enemies[i]->drawEnemy(&canvas, cameraX, cameraY);     // In enemy parent class
			}
		}
	}

	int getGhostType()                 // getter used for getting ghostType, used for saving
	{
		return ghostNumberType;
	}

	
	void moveAll(Player& player, GamesEngineeringBase::Window& canvas)          // move all enemies in enemy manager
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead())              // if enemy is not a nullptr and enemy is not dead
			{
				enemies[i]->moveEnemyTowardsPlayer(player);
			}
		}
	}

	void enemyCollisionPlayer(Player& player)      // collision between enemy and player
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead() && enemies[i]->enemyCollidePlayer(player))
			{
				enemies[i]->setEnemyDead();
			}
		}
	}

	void enemyCollisionBullet(Player& player)      // collision between enemy and bullet
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead() && enemies[i]->enemyCollideBullet(player))   // if the enemy is not a nullptr and not dead and bullet collision
			{
				enemies[i]->setEnemyDead();                                      // The enemyAlive for that enemy at that index in the array is set to false so enemy is not drawn
			}
		}
	}

	void enemyCollisionAbility(Player& player)      // collision between enemy and ability
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead() && enemies[i]->enemyCollideAbility(player))   // if the enemy is not a nullptr and not dead and bullet collision
			{
				enemies[i]->setEnemyDead();                                      // The enemyAlive for that enemy at that index in the array is set to false so enemy is not drawn
			}
		}
	}

	void updateEnemyProjectile(GamesEngineeringBase::Window& canvas, float deltaTime, Player& player, float cameraX, float cameraY)
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead())     // if enemies exist and enemies are not dead 
			{
				enemies[i]->showEnemyProjectile(canvas, player, deltaTime, cameraX, cameraY);
			}
		}
	}

	void projectileCollisionPlayer(Player& player)
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && !enemies[i]->isEnemyDead() && enemies[i]->projectileCollidePlayer(player))
			{
				enemies[i]->setProjectileDead();
			}
		}
	}

	void deleteDeadEnemies()      // find enemies marked as dead and remove them from memory
	{
		for (unsigned int i = 0; i < count; i++)
		{
			if (enemies[i] != nullptr && enemies[i]->isEnemyDead())   // if enemy is not a nullptr and enemy is dead
			{
				delete enemies[i];                 // delete enemy from memory
				enemies[i] = nullptr;              // make that enemy slot a nullptr
				enemies[i] = enemies[count - 1];   // Move the last alive enemy into this empty slot
				enemies[count - 1] = nullptr;      // set the slot where the enemy came from to a null ptr
				count--;                           // reduce the count so we now have one less enemy in the game
			}
		}
	}
};

	

