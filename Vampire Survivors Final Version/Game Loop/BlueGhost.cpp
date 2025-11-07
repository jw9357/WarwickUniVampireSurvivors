#include "GamesEngineeringBase.h"
#include <random>
#include "BlueGhost.h"


// Default Constructor
BlueGhost::BlueGhost()
{

}

// Constructor with paramters initializes the spawning in of Blue Ghost and sets them to alive
BlueGhost::BlueGhost(float _x, float _y, GamesEngineeringBase::Window& canvas)
{
	enemyRightImage.load("Resources/EnemyBlueRight.png");
	enemyLeftImage.load("Resources/EnemyBlueLeft.png");

	srand(static_cast<unsigned>(time(0)));    

	spawnEnemy(canvas);
	enemyAlive = true;

	moveSpeed = 15.0f;

	health = 50.0f;
}

// Destructor
BlueGhost::~BlueGhost()
{

}

bool BlueGhost::enemyCollideBullet(Player& player)
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

		player.bulletActive = false;             // bullet is not drawn but still exists in memory

		if (health <= 0)
		{
			//std::cout << "Enemy Dead" << std::endl;
			player.score += 100;
			std::cout << "Current Player score: " << player.score << std::endl;

			return true;
		}
	}
	return false;
}




