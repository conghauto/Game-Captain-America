#include "Soldier.h"
#include "define.h"

void Soldier::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SOLDIER_BBOX_WIDTH;

	if (state == SOLDIER_STATE_DIE)
		bottom = y + SOLDIER_BBOX_HEIGHT_DIE;
	else
		bottom = y + SOLDIER_BBOX_HEIGHT;
}

void Soldier::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (state == SOLDIER_STATE_DIE)
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

void Soldier::Render()
{
	int ani;

	if (state == SOLDIER_STATE_DIE) {
		if (nx > 0) {
			ani = SOLDIER_ANI_DIE_RIGHT;
		}
		else
		{
			ani = SOLDIER_ANI_DIE_LEFT;
		}

		return;
	}
	else {
		if (vx < 0)
			ani = SOLDIER_ANI_WALKING_LEFT;
		else if (vx > 0)
			ani = SOLDIER_ANI_WALKING_RIGHT;

		animations[ani]->Render(x, y);
	}
	RenderBoundingBox();
}

void Soldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SOLDIER_STATE_DIE:
		y += SOLDIER_BBOX_HEIGHT - SOLDIER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case SOLDIER_STATE_WALKING:
		vx = SOLDIER_WALKING_SPEED;
	}

}

