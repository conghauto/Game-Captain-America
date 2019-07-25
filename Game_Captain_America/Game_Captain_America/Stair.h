#pragma once

#include "GameObject.h"

#define STAIR_BBOX_WIDTH  32
#define STAIR_BBOX_HEIGHT 32

class Stair : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

