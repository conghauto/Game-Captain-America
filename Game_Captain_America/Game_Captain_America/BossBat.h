#pragma once
#include "GameObject.h"
#include"Game.h"

class BossBat : public CGameObject
{
	bool startAttack = false;
	DWORD wait = 0;
	DWORD startAttack1 = 0;
	bool check = false;
public:
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Update(float xs, float ys, DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};
