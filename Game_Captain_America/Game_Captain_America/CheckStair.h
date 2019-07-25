#pragma once

#include "GameObject.h"

class CheckStair : public CGameObject
{
	int type;

public:
	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
