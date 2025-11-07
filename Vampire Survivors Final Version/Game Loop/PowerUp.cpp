#include "GamesEngineeringBase.h"
#include <iostream>
#include <algorithm> 
#include <random>
#include "PowerUp.h"
using namespace std;

// Default Constructor
PowerUp::PowerUp()
{
	
}

// Constructor with paramters initializes the position of the powerup
PowerUp::PowerUp(GamesEngineeringBase::Window& canvas)
{
	srand(static_cast<unsigned>(time(0)));

	powerUpX = 0;
	powerUpY = 0;
	
	powerUpImage.load("Resources/purpleClock.png");

	canDrawPowerUp = false;
	stopPowerUpFromAppearing = false;
	hasPowerUpBeenCollected = false;
}

// Destructor
PowerUp::~PowerUp()
{

}

void PowerUp::showPowerUp(GamesEngineeringBase::Window& canvas, float deltaTime)
{
	if (!canDrawPowerUp)
	{
		drawPowerUpTimer -= deltaTime;        // start countdown timer

		if (drawPowerUpTimer < 0.0f && !canDrawPowerUp)      // if timer reaches zero and canDrawPowerUp is false
		{
			spawnPowerUp(canvas);          // draw power up on screen

			drawPowerUpTimer = 0.0f;      // power up only shows once in game
		}
	}
}

float PowerUp::getDrawPowerUpTimer(float drawPowerUpTimer)
{
	return drawPowerUpTimer;
}

void PowerUp::spawnPowerUp(GamesEngineeringBase::Window& canvas)
{
	if (!canDrawPowerUp)
	{
		random_device randomSeed;          // Create a random device for seeding
		mt19937 gen(randomSeed());         // Uses a Mersenne Twister which is a really long sequence which doesn't repeat

		uniform_int_distribution<> distrX(550, 1900);   // Ensures the numbers are within the world x co-ordinate space the player can move in
		uniform_int_distribution<> distrY(400, 2100);   // Ensures the numbers are within the world y co-ordinate space the player can move in

		powerUpX = static_cast<float>(distrX(gen));    // sets the powerUp at a random position within the space the player can move
		powerUpY = static_cast<float>(distrY(gen));

		canDrawPowerUp = true;          // draw powerUp on screen at a random world position
	}
}

void PowerUp::drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas)
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

void PowerUp::drawPowerUp(GamesEngineeringBase::Window* canvas, float cameraX, float cameraY)
{
	// Converts world co-ordinates to screen co-ordinates
	screenX = powerUpX - cameraX;
	screenY = powerUpY - cameraY;

	
	if (canDrawPowerUp && !stopPowerUpFromAppearing)
	{
		for (int y = 0; y < powerUpImage.height; y++)
		{
			float drawY = screenY + y;                            // bounds check on y, if not crash when powerup off screen
			if (drawY > 0 && drawY < canvas->getHeight())
			{
				for (int x = 0; x < powerUpImage.width; x++)
				{
					float drawX = screenX + x;                        // bounds check on x, if not crash when powerup off screen
					if (drawX > 0 && drawX < canvas->getWidth())
					{
						if (powerUpImage.alphaAt(x, y) > 0)
						{
							canvas->draw(static_cast<int>(screenX) + x, static_cast<int>(screenY) + y, powerUpImage.at(x, y));
						}
					}
				}
			}
		}

		//drawCollisionCircle(screenX + powerUpImage.width / 2, screenY + powerUpImage.height / 2, 30, *canvas);
	}
}

void PowerUp::powerUpCollidePlayer(Player& player)
{
	// uses world co-ords of powerUP and world co-ords of player 
	float dx = (player.playerX + player.cameraX) - powerUpX;
	float dy = (player.playerY + player.cameraY) - powerUpY;

	float radius = 70;
	float radiusSquared = radius * radius;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < radiusSquared)
	{
		std::cout << "Player hit powerup" << std::endl;
		canDrawPowerUp = false;
		stopPowerUpFromAppearing = true;
		hasPowerUpBeenCollected = true;
	}
}

