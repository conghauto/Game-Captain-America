#include "CheckPoint.h"
#include "define.h"

void CheckPoint::Render()
{
	//RenderBoundingBox();
}

void CheckPoint::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = 0;
	r = x + CHECKSTAIR_BBOX_WIDTH;
	b = y + CHECKSTAIR_BBOX_HEIGHT * 100;
}
