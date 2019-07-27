#pragma once
#include "GameObject.h"
#include "Ground.h"

class Item : public CGameObject
{

public:
	int type;
	bool isEaten;
	DWORD appearTime;
	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

