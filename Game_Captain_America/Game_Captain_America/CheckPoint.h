#pragma once
#include "GameObject.h"

class CheckPoint : public CGameObject
{
	int type;
public:
	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};