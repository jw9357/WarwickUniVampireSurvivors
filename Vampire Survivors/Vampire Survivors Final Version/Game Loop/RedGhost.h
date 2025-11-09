#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"

class Player;   // forward declare Player

class RedGhost : public Enemy
{
private:

protected:
	int health;

public:
	bool enemyAlive = true;
	bool lookingRight = true;

	// Default Constructor
	RedGhost();

	// Constructor with paramters initializes the spawning in of RedGhost and sets them to alive
	RedGhost(float _x, float _y, GamesEngineeringBase::Window& canvas);

	// Destructor
	~RedGhost();

	bool enemyCollideBullet(Player& player) override;

};