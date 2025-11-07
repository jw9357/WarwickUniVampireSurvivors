#include "GamesEngineeringBase.h"
#include <random>
#include "Demon.h"


// Default Constructor
Demon::Demon()
{

}

// Constructor with paramters initializes the spawning in of Red Ghost and sets them to alive
Demon::Demon(float _x, float _y, GamesEngineeringBase::Window& canvas)
{
	
	enemyRightImage.load("Resources/EnemyDemonRight.png");
	enemyLeftImage.load("Resources/EnemyDemonLeft.png");

	srand(static_cast<unsigned>(time(0)));  

	spawnEnemy(canvas);
	enemyAlive = true;

	moveSpeed = 5.0f;

	health = 200.0f;
}

// Destructor
Demon::~Demon()
{

}

bool Demon::enemyCollideBullet(Player& player)
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
			player.score += 250;
			std::cout << "Current Player score: " << player.score << std::endl;

			return true;
		}
	}
	return false;
}


