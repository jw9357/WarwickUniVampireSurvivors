#include "GamesEngineeringBase.h"
#include <random>
#include "RedGhost.h"


// Default Constructor
RedGhost::RedGhost()
{

}

// Constructor with paramters initializes the spawning in of Red Ghost and sets them to alive
RedGhost::RedGhost(float _x, float _y, GamesEngineeringBase::Window& canvas)
{
	
	enemyRightImage.load("Resources/EnemyRedRight.png");
	enemyLeftImage.load("Resources/EnemyRedLeft.png");

	srand(static_cast<unsigned>(time(0)));    

	spawnEnemy(canvas);
	enemyAlive = true;

	moveSpeed = 10.0f;                             

	health = 100.0f;
}

// Destructor
RedGhost::~RedGhost()
{

}

bool RedGhost::enemyCollideBullet(Player& player)
{
	float dx = (player.bulletX + player.cameraX) - enemyX;                    
	float dy = (player.bulletY + player.cameraY) - enemyY;

	float radius = 40.0f;
	float radiusSquared = radius * radius;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < radiusSquared)
	{
		if (!player.bulletActive)               
		{
			return false;
		}

		const unsigned int damage = 50;
		health -= damage;
		//std::cout << "Enemy Hit, health is now " << health << std::endl;

		player.bulletActive = false;            

		if (health <= 0)
		{
			//std::cout << "Enemy Dead" << std::endl;
			player.score += 150;
			std::cout << "Current Player score: " << player.score << std::endl;

			return true;
		}
	}
	return false;
}

