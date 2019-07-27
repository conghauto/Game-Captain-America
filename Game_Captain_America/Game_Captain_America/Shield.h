#pragma once
#include "Weapon.h"

class Shield : public Weapon
{

public:
	Shield(Captain *captain, float max_width = SCREEN_WIDTH) { this->captain = captain; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};