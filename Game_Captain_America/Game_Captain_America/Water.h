#pragma once
#include "GameObject.h"
#include"define.h"


class Water : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

