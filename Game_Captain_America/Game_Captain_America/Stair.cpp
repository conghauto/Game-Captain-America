#include "Stair.h"


void Stair::Render()
{
	//animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Stair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + STAIR_BBOX_WIDTH;
	b = y + STAIR_BBOX_HEIGHT;
}