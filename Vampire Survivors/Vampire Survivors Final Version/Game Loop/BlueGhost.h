#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"

class Player;   // forward declare Player

class BlueGhost : public Enemy
{
private:

protected:
	int health;

public:
	bool enemyAlive = true;
	bool lookingRight = true;

	// Default Constructor
	BlueGhost();

	// Constructor with paramters initializes the spawning in of RedGhost and sets them to alive
	BlueGhost(float _x, float _y, GamesEngineeringBase::Window& canvas);

	// Destructor
	~BlueGhost();

	bool enemyCollideBullet(Player& player) override;

};