#pragma once
#include "Item.h"
#include "Zombie.h"
#include "Captain.h"


class Weapon : public Item
{

public:
	float max_width;
	float max_x;
	Captain *captain;

	bool isActivate;
	DWORD firstCast;

	Weapon() {}

	Weapon(Captain *captain, float max_width = SCREEN_WIDTH) { this->captain = captain; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
};


