#include "Lantern.h"


void Lantern::Render()
{
	if (!isShooted)
		animations[0]->Render(x, y, 255);
}
void Lantern::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Lantern::SetShoot()
{
	isShooted = true;
}

void Lantern::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + LANTERN_BBOX_WIDTH;
	b = y + LANTERN_BBOX_HEIGHT;
}
