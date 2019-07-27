#include "Water.h"

void Water::Render()
{
	animations[0]->Render(x, y, 255);
	RenderBoundingBox();
}

void Water::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + WATER_BBOX_WIDTH;
	b = y + WATER_BBOX_HEIGHT;
}
