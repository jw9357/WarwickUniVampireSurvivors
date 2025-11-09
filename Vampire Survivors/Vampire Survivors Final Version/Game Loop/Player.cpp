
#include "GamesEngineeringBase.h"
#include <iostream>
#include <algorithm> 
#include "Player.h"
#include "PowerUp.h"
using namespace std;

// Constructor with paramters initializes the position of the player in the middle of the screen
Player::Player(GamesEngineeringBase::Window& canvas)
{
	// Player always set to centre of screen
	playerX = static_cast<float>(canvas.getWidth() / 2);
	playerY = static_cast<float>(canvas.getHeight() / 2);
	//std::cout << "Player initialized at (" << playerX << ", " << playerY << ")" << std::endl;

	// Player camera position, moveable on key presses
	cameraX = 0;  
	cameraY = 0; 
	//std::cout << "Player camera position at (" << cameraX << ", " << cameraY << ")" << std::endl;


	// Load in images of player, bullets and ability
	playerRightImage.load("Resources/HeroRight.png");     // load image of player looking right
	playerLeftImage.load("Resources/HeroLeft.png");       // load image of player looking left
	bulletImage.load("Resources/bulletC.png");            // load image of bullet
	abilityImage.load("Resources/FlameA.png");            // load image of special ability
}

void Player::drawCollisionCircle(float centreX, float centreY, float radius, GamesEngineeringBase::Window& canvas)
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

void Player::drawPlayer(GamesEngineeringBase::Window* canvas, float playerX, float playerY)
{
	if (playerAlive)
	{
		GamesEngineeringBase::Image* currentDirectionImage;    // pointer to image object

		if (lookingRight)
		{
			currentDirectionImage = &playerRightImage;        // if looking right, use this image at this address
		}
		else
		{
			currentDirectionImage = &playerLeftImage;         // if looking left, use this image at this address
		}

		// Draw whichever image is being called above
		for (unsigned int y = 0; y < currentDirectionImage->height; y++)
		{
			for (unsigned int x = 0; x < currentDirectionImage->width; x++)
			{
				if (currentDirectionImage->alphaAt(x, y) > 0)
				{
					canvas->draw(static_cast<int>(playerX) + x, static_cast<int>(playerY) + y, currentDirectionImage->at(x, y));
				}
			}
		}

		//drawCollisionCircle(playerX + playerRightImage.width / 2, playerY + playerLeftImage.height / 2, 30, *canvas);
	}
	else
	{
		std::cout << "GAME OVER" << std::endl;
		std::cout << "TOTAL SCORE is: " << score << std::endl;
	}
}

void Player::lookLeft()
{
	lookingRight = false;
}

void Player::lookRight()
{
	lookingRight = true;
}

	
void Player::drawBullet(float bulletX, float bulletY, GamesEngineeringBase::Window& canvas)
{
	if (bulletActive)
	{
		//drawCollisionCircle(bulletX + bulletImage.width / 2, bulletY + bulletImage.height / 2, 10, canvas);

		for (unsigned int y = 0; y < bulletImage.height; y++)      // draws every y pixel and every x pixel of image on screen
		{
			for (unsigned int x = 0; x < bulletImage.width; x++)
			{
				if (bulletImage.alphaAt(x, y) > 0)         // if the alpha at that co-ordinate is not zero
				{
					canvas.draw(static_cast<int>(bulletX) + x, static_cast<int>(bulletY) + y, bulletImage.at(x, y));
				}
			}
		}
	}
}

void Player::shoot()
{
	bulletActive = true;
	bulletRight = lookingRight;                    // when bullet is fired, it matches the direction the player is facing

	if (bulletRight)
	{
		bulletX = playerX + (playerRightImage.width) - (bulletImage.width / 2);
		bulletY = playerY + (playerRightImage.height / 2) + (bulletImage.height / 2);
	}
	else
	{
		bulletX = playerX + (playerLeftImage.width / 2) + (bulletImage.width / 2); 
		bulletY = playerY + (playerLeftImage.height / 2);  
	}
	
}

void Player::moveBullet(GamesEngineeringBase::Window& canvas)
{
	if (bulletActive)
	{
		if (bulletRight)
		{
			bulletX += bulletSpeed;      // moves bullet to the right of the screen
		}
		else
		{
			bulletX -= bulletSpeed;      // moves bullet to the left of the screen
		}
	}

	if (bulletX + bulletImage.width <= 0 || bulletX >= canvas.getWidth())
	{
		bulletActive = false;
	}
}

void Player::activateAbility()
{
	drawSpecialAbilityActive = true;
}

void Player::drawAbility(float abilityX, float abilityY, GamesEngineeringBase::Window& canvas, float deltaTime)
{
	if (!abilityReady)     // if ability is not ready, do nothing when the ability button is pushed
	{
		return;
	}

	if (drawSpecialAbilityActive && abilityReady)   // draws 4 abilities on screen
	{
		for (unsigned int y = 0; y < abilityImage.height; y++)
		{
			for (unsigned int x = 0; x < abilityImage.width; x++)
			{
				if (abilityImage.alphaAt(x, y) > 0)
				{
					canvas.draw(static_cast<int>(abilityX) + 200 + x, static_cast<int>(abilityY) + 200 + y, abilityImage.at(x, y));
					canvas.draw(static_cast<int>(abilityX) + 700 + x, static_cast<int>(abilityY) + 200 + y, abilityImage.at(x, y));
					canvas.draw(static_cast<int>(abilityX) + 500 + x, static_cast<int>(abilityY) + 400 + y, abilityImage.at(x, y));
					canvas.draw(static_cast<int>(abilityX) + 500 + x, static_cast<int>(abilityY) + 100 + y, abilityImage.at(x, y));
				}
			}
		}
	}
}

void Player::startAbilityDrawCooldown(float deltaTime, const PowerUp& powerUp)
{
	if (drawSpecialAbilityActive)
	{
		cooldownShowingAbility -= deltaTime;        // timer counts down showing ability on screen

		if (cooldownShowingAbility <= 0.0f)
		{
			drawSpecialAbilityActive = false;
			abilityReady = true;
			cooldownShowingAbility = 0.0f;
			resetActive = true;

			// If PowerUp is collected by player, then special ability reset timer is changed from 15s to 5s
			
			if (powerUp.hasPowerUpBeenCollected)
			{
				cooldownTilNextButtonPress = 5.0f;
			}
			else
			{
				cooldownTilNextButtonPress = 15.0f;
			}
		}
	}
}

void Player::startAbilityResetCooldown(float deltaTime)
{
	if (resetActive)
	{
		abilityReady = false;
		
		// Starts Special Ability countodnw timer
		cooldownTilNextButtonPress -= deltaTime;
		
		if (cooldownTilNextButtonPress <= 0.0f)
		{
			cooldownTilNextButtonPress = 0.0f;
			resetActive = false;
			abilityReady = true;
			cooldownShowingAbility = 2.0f;
		}
	}
}

void Player::Dead()
{ 
	playerAlive = false; 
	std::cout << "Player has died" << std::endl;

}

bool Player::isAlive() const
{ 
	return playerAlive; 
}

void Player::setScore(int _score)
{
	score = _score;   // object = argument
}

void Player::setHealth(int _health)
{
	health = _health;   // object = argument
}

void Player::setPosition(float _playerX, float _cameraX, float _playerY, float _cameraY)
{
	playerX = _playerX;
	cameraX = _cameraX;
	playerY = _playerY;
	cameraY = _cameraY;
}

