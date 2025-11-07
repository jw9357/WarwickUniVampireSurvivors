#pragma once

#include "GamesEngineeringBase.h"
#include "Player.h"

class Player;   // forward declare Player

class WhiteGhost : public Enemy
{
private:

protected:
	float health;

public:
	bool enemyAlive = true;
	bool lookingRight = true;

	WhiteGhost();

	WhiteGhost(float _x, float _Y, GamesEngineeringBase::Window& canvas);

	~WhiteGhost();

	bool enemyCollideBullet(Player& player) override;


};