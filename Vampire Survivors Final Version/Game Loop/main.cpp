#include <iostream>
#include "GamesEngineeringBase.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include <random>
#include <algorithm> 
#include "EnemyManager.cpp"
#include "TileSet.cpp"
#include <fstream>
#include <cmath>
using namespace std;

// Save game function
void saveGame(const Player& player, EnemyManager& enemyManager)
{
	cout << "Saving game" << endl;

	ofstream outFile("SavedGame.txt");   // prints everythig to this text document

	if (outFile)
	{
		// Player Data
		outFile << player.score << endl;
		outFile << player.health << endl;
		outFile << player.playerX << endl;
		outFile << player.cameraX << endl;
		outFile << player.playerY << endl;
		outFile << player.cameraY << endl;

		// Enemy Data
		int enemyCount = enemyManager.getEnemyCount();    // stores enemy count i.e. active enemies in game
		outFile << enemyCount << endl;

		Enemy** arrayOfEnemies = enemyManager.getEnemiesArray();

		// Access every individual enemy for their x and y world locations
		for (int i = 0; i < enemyCount; i++)
		{
			Enemy* ptr = arrayOfEnemies[i];

			outFile << ptr->getEnemyX() << endl;
			outFile << ptr->getEnemyY() << endl;
			outFile << ptr->getProjectileX() << endl;
			outFile << ptr->getProjectileY() << endl;
		}
	}
	else
	{
		cout << "Couldn't create save file" << endl;
	}

	outFile.close();
}

void loadGame(Player& player, EnemyManager& enemyManager)
{
	ifstream inFile("SavedGame.txt");   // reads the txt file

	if (inFile)
	{
		int score;
		inFile >> score;
		player.setScore(score);

		int health;
		inFile >> health;
		player.setHealth(health);

		float playerX, cameraX, playerY, cameraY;
		inFile >> playerX >> cameraX >> playerY >> cameraY;
		player.setPosition(playerX, cameraX, playerY, cameraY);

		int enemyCount = enemyManager.getEnemyCount();
		enemyCount = 0;                               

		/*for (int i = 0; i < enemyCount; i++)
		{
			float enemyX, enemyY, projectileX, projectileY;
			inFile >> enemyX >> enemyY >> projectileX >> projectileY;
			Enemy* ptr = new Enemy();
			enemyManager.add(ptr);
		}*/
	}

}

void newMap(Player& player, EnemyManager& enemyManager, PowerUp& powerUp)
{
	player.setScore(0);               // set score back to zero
	player.setHealth(500);            // set player health back to 500
	std::cout << "New Map Loaded" << std::endl;
	player.playerX = 512;     // SHOULD BE CANVAS WIDTH / 2
	player.cameraX = 0;       // camera X reset to 0
	player.playerY = 384;      // SHOULD BE CANVAS HEIGHT / 2
	player.cameraY = 0;        // camera Y reset to 0
	
	Enemy** arrayOfEnemies = enemyManager.getEnemiesArray();
	int enemyCount = enemyManager.getEnemyCount();

	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* ptr = arrayOfEnemies[i];
		ptr->setEnemyDead();                // set all enemies that are currently active in game to dead which will delete them from the game
	}

	enemyCount = 0;                         // number of enemies in game goes back to zero



	// Set enemy spawn delay back to 7 seconds
	float spawnDelay = enemyManager.getSpawnDelay(7.0f);
	
	// Delete powerUp from game if active
	if (powerUp.canDrawPowerUp)
	{
		powerUp.canDrawPowerUp = false;
	}
	
	// Set drawPowerUpTimer back to 60 seconds
	float powerUpTimer = powerUp.getDrawPowerUpTimer(60.0f);
	
}


int main() 
{
	// Create a canvas window with dimensions 1024x768 and title “Tiles"
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "Vampire Survivors");

	// Create instance of player
	Player player(canvas);

	// Set initial player position
	player.cameraX;
	player.cameraY;

	// Set player move speed in game i.e speed the map moves
	int moveSpeed = 300;

	// Create instance of Enemy Manager
	EnemyManager enemyManger;

	// Create instance of PowerUp
	PowerUp powerUp(canvas);

	// Create instance of TileSet
	tileSet ts;

	// Variables for Map
	const unsigned int rows = 80;
	const unsigned int columns = 80;
	const unsigned int tileSize = 32;
	int mapData[rows][columns];


	// Reads the map file
	ifstream file("tilesC.txt");

	// Goes through the file above and reads each character one by one and fills in the MapData
	for (unsigned int y = 0; y < rows; y++)
	{
		for (unsigned int x = 0; x < columns; x++)
		{
			if (!(file >> mapData[y][x]))
			{
				cerr << "Error" << endl;
				return 1;
			}
		}
	}
	file.close();

	// deltaTime
	GamesEngineeringBase::Timer tim;  

	// FPS variables
	float timeElapsed = 0;
	unsigned int framesElapsed = 0;

	// Loads game
	loadGame(player, enemyManger);
	
	// Variable to control the main loop's running state.
	bool running = true;
	while (running)
	{
		// DelaTime in game
		float dt = tim.dt();

		// FPS functionality
		timeElapsed += dt;
		framesElapsed++;
		if (timeElapsed > 1)
		{
			std::cout << "Frames count: " << framesElapsed << std::endl;
			timeElapsed = 0.0f;
			framesElapsed = 0;
		}

		// Player ability functions
		player.startAbilityDrawCooldown(dt, powerUp);
		player.startAbilityResetCooldown(dt);

		// Check for input (key presses or window events)
		canvas.checkInput();

		// ESCAPE KEY - Exits Game
		if (canvas.keyPressed(VK_ESCAPE))
		{
			break;
		}

		if (canvas.keyPressed('N'))
		{
			newMap(player, enemyManger, powerUp);
		}

		// PLAYER MOVING LEFT
		if (canvas.keyPressed(VK_LEFT))
		{
			float lastPositionX = player.cameraX;
			player.lookLeft();
			player.cameraX -= moveSpeed * dt;

			float worldPositionX = player.playerX + player.cameraX;
			float worldPositionY = player.playerY + player.cameraY;

			if (worldPositionX < canvas.getWidth() / 2)
			{
				player.cameraX = lastPositionX;
			}

			// Stop player going through water tile if going left
			float possibleWaterTileX = worldPositionX / tileSize;
			float possibleWaterTileY = worldPositionY / tileSize;
			int roundedTileX = static_cast<int>(round(possibleWaterTileX));
			int roundedTileY = static_cast<int>(round(possibleWaterTileY));
			int tileCentreX = roundedTileX * tileSize + tileSize / 2;
			int tileCentreY = roundedTileY * tileSize + tileSize / 2;

			if (mapData[roundedTileY][roundedTileX] == 1)
			{
				float dx = worldPositionX - tileCentreX;
				float dy = worldPositionY - tileCentreY;
				float tileRadius = tileSize / 2;
				float radiusSum = player.playerRightImage.width / 2 + tileRadius;
				float distanceSquared = dx * dx + dy * dy;

				if (distanceSquared < radiusSum * radiusSum)
				{
					player.cameraX = lastPositionX;
				}
			}
		
		}
		// PLAYER MOVING RIGHT
		if (canvas.keyPressed(VK_RIGHT))
		{
			float lastPositionX = player.cameraX;
			player.lookRight();
			player.cameraX += moveSpeed * dt;

			float worldPositionX = player.playerX + player.cameraX + player.playerRightImage.width / 2 ;
			float worldPositionY = player.playerY + player.cameraY;

			if (worldPositionX > (rows * tileSize) - canvas.getWidth() / 2)
			{
				player.cameraX = lastPositionX;
			}

			// Stop player going through water tile if going right
			float possibleWaterTileX = worldPositionX / tileSize;
			float possibleWaterTileY = worldPositionY / tileSize;
			int roundedTileX = static_cast<int>(round(possibleWaterTileX));
			int roundedTileY = static_cast<int>(round(possibleWaterTileY));
			int tileCentreX = roundedTileX * tileSize - (tileSize / 2);
			int tileCentreY = roundedTileY * tileSize - (tileSize / 2);

			if (mapData[roundedTileY][roundedTileX] == 1)
			{
				float dx = worldPositionX - tileCentreX;
				float dy = worldPositionY - tileCentreY;
				float tileRadius = tileSize / 2;
				float radiusSum = player.playerRightImage.width / 2 + tileRadius;
				float distanceSquared = dx * dx + dy * dy;

				if (distanceSquared < radiusSum * radiusSum)
				{
					player.cameraX = lastPositionX;
				}
			}
		}

		// PLAYER MOVING UP
		if (canvas.keyPressed(VK_UP))
		{
			float lastPositionY = player.cameraY;
			player.cameraY -= moveSpeed * dt;

			float worldPositionX = player.playerX + player.cameraX;
			float worldPositionY = player.playerY + player.cameraY;
			
			if (worldPositionY < canvas.getHeight() / 2)
			{
				player.cameraY = lastPositionY;
			}

			// Stop player going through water tile if going up
			float possibleWaterTileX = worldPositionX / tileSize;
			float possibleWaterTileY = worldPositionY / tileSize;
			int roundedTileX = static_cast<int>(round(possibleWaterTileX));
			int roundedTileY = static_cast<int>(round(possibleWaterTileY));
			int tileCentreX = roundedTileX * tileSize + (tileSize / 2);
			int tileCentreY = roundedTileY * tileSize + (tileSize / 2);

			if (mapData[roundedTileY][roundedTileX] == 1)
			{
				float dx = worldPositionX - tileCentreX;
				float dy = worldPositionY - tileCentreY;
				float tileRadius = tileSize / 2;
				float radiusSum = player.playerRightImage.width / 2 + tileRadius;
				float distanceSquared = dx * dx + dy * dy;

				if (distanceSquared < radiusSum * radiusSum)
				{
					player.cameraY = lastPositionY;
				}
			}
		}

		// PLAYER MOVING DOWN
		if (canvas.keyPressed(VK_DOWN))
		{
			float lastPositionY = player.cameraY;
			player.cameraY += moveSpeed * dt;

			float worldPositionX = player.playerX + player.cameraX;
			float worldPositionY = player.playerY + player.cameraY + player.playerLeftImage.height;

			if (worldPositionY > (columns * tileSize) - canvas.getHeight() / 2)
			{
				player.cameraY = lastPositionY;
			}

			// Stop player going through water tile if going down
			float possibleWaterTileX = worldPositionX / tileSize;
			float possibleWaterTileY = worldPositionY / tileSize;
			int roundedTileX = static_cast<int>(floor(possibleWaterTileX));
			int roundedTileY = static_cast<int>(floor(possibleWaterTileY));
			int tileCentreX = roundedTileX * tileSize + (tileSize / 2);
			int tileCentreY = roundedTileY * tileSize + (tileSize / 2);

			if (mapData[roundedTileY][roundedTileX] == 1)
			{
				float dx = worldPositionX - tileCentreX;
				float dy = worldPositionY - tileCentreY;
				float tileRadius = tileSize / 2;
				float radiusSum = player.playerRightImage.width / 2 + tileRadius;
				float distanceSquared = dx * dx + dy * dy;

				if (distanceSquared < radiusSum * radiusSum)
				{
					player.cameraY = lastPositionY;
				}
			}
		}

		// Input Key for bullet
		if (canvas.keyPressed(VK_SPACE) && !player.bulletActive)
		{
			//Bullet fires in direction player is facing
			player.shoot();
		}

		if (canvas.keyPressed('X'))
		{
			//Player special ability
			player.activateAbility();
		}

		// Clear the window for the next frame rendering
		canvas.clear();

		// Draws a black screen over the whole screen                // for every single x and y, draw a blue pixel
		for (unsigned int y = 0; y < canvas.getHeight(); y++)
			for (unsigned int x = 0; x < canvas.getWidth(); x++)
				canvas.draw(x, y, 0, 0, 0);
		

		// Draw tiles using mapData
		for (unsigned int y = 0; y < rows; y++)
		{
			for (unsigned int x = 0; x < columns; x++)
			{
				int tileIndex = mapData[y][x];
				ts[tileIndex].draw(canvas, ((x * tileSize) - static_cast<int>(player.cameraX)), ((y * tileSize) - static_cast<int>(player.cameraY)));   // map moves in opposite direction of player key press
			}
		}

		// Draw All Enemies in Enemy manager to the canvas
		enemyManger.drawAll(canvas, dt, player.cameraX, player.cameraY);

		// Deals with Shooter enemy throwing a projectile
		enemyManger.updateEnemyProjectile(canvas, dt, player, player.cameraX, player.cameraY);

		// Move All Enemies in Enemy manager to the canvas
		enemyManger.moveAll(player, canvas);

		// Move bullet
		player.moveBullet(canvas);

		// Player collides with Power up
		powerUp.powerUpCollidePlayer(player);

		// Check bullet collison with enemy
		enemyManger.enemyCollisionBullet(player);

		// Check ability collison with enemy
		enemyManger.enemyCollisionAbility(player);

		// Check shooter projectile collison with player
		enemyManger.projectileCollisionPlayer(player);

		// Clean up dead enemies
		enemyManger.deleteDeadEnemies();

		// Check Enemy collision with Player	
		enemyManger.enemyCollisionPlayer(player);

		// If Player is dead, stop the game from running
		if (!player.playerAlive)
		{
			running = false;

		}

		// Draw Player at centre of screen, screen position
		player.drawPlayer(&canvas, player.playerX, player.playerY);
		//std::cout << "Player World co-ordinates at (" << player.playerX + player.cameraX << ", " << player.playerY + player.cameraY << ")" << std::endl;
		//std::cout << "Player centre co-ordinates at (" << player.cameraX << ", " << player.cameraY << ")" << std::endl;

		// Draw Bullet on screen
		if (player.bulletActive)
		{
			player.drawBullet(player.bulletX, player.bulletY, canvas);
		}

		// Draw Ability on screen
		if (player.drawSpecialAbilityActive)
		{
			player.drawAbility(0, 0, canvas, dt);
		}

		// Timer for PowerUp to show on screen
		powerUp.showPowerUp(canvas, dt);
		
		// Draw Power Up
		powerUp.drawPowerUp(&canvas, player.cameraX, player.cameraY);

		// Display the frame on the screen. This must be called once the frame is finished in order to display the frame.
		canvas.present();
	}

	// Save game called from outside main loop at top only if player is alive
	if (player.playerAlive)
	{
		saveGame(player, enemyManger);
	}

	return 0;
}


