#include "Zombie.h"
#include "define.h"

void Zombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;

	if (state == ZOMBIE_STATE_DIE)
		bottom = y + ZOMBIE_BBOX_HEIGHT_DIE;
	else
		bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (state == ZOMBIE_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > MAX_WIDTH_LV2) {
		x = MAX_WIDTH_LV2; 
		vx = -vx;
	}


}

void Zombie::Render()
{
	int ani ;
	
	if (state == ZOMBIE_STATE_DIE) {
		if (nx > 0) {
			ani = ZOMBIE_ANI_DIE_RIGHT;
		}
		else
		{
			ani = ZOMBIE_ANI_DIE_LEFT;
		}

		return;
	}
	else {
		if (vx < 0)
			ani = ZOMBIE_ANI_WALKING_LEFT;
		else if (vx > 0)
			ani = ZOMBIE_ANI_WALKING_RIGHT;

		animations[ani]->Render(x, y);
	}
	RenderBoundingBox();
}

void Zombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_DIE:
		y += ZOMBIE_BBOX_HEIGHT - ZOMBIE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case ZOMBIE_STATE_WALKING:
		vx = -ZOMBIE_WALKING_SPEED;
	}

}

