#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"

class Player;   // forward declare Player

class Demon : public Enemy
{
private:

protected:
	int health;

public:
	bool enemyAlive = true;
	bool lookingRight = true;

	// Default Constructor
	Demon();

	// Constructor with paramters initializes the spawning in of RedGhost and sets them to alive
	Demon(float _x, float _y, GamesEngineeringBase::Window& canvas);

	// Destructor
	~Demon();

	bool enemyCollideBullet(Player& player) override;

};