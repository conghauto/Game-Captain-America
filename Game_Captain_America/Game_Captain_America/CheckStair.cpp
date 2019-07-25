#include "CheckStair.h"
#include "define.h"

void CheckStair::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CheckStair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + CHECKSTAIR_BBOX_WIDTH;
	b = y + CHECKSTAIR_BBOX_HEIGHT;
}
