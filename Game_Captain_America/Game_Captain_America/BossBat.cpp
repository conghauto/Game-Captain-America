#include "BossBat.h"
#include "define.h"
#include <math.h>
void BossBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOSSBAT_BBOX_WIDTH;	
	bottom = y + BOSSBAT_BBOX_HEIGHT;
}

void BossBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	this->x += dx;
	this->y += dy;
	/*if (CGame::GetInstance()->x_cam == MAX_WIDTH_LV2_2 - SCREEN_WIDTH && check == false)
	{
		startAttack = true;
		wait = GetTickCount();
		check = true;
	}
	if (startAttack == true && check == true)
		if(GetTickCount() - wait >3000)
		this->SetState(BOSSBAT_STATE_ATTACK);*/
}

void BossBat::Update(float xs,float ys,DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	this->x += dx;
	this->y += dy;
	if (xs >5400 && check == false)
	{
		startAttack = true;
		wait = GetTickCount();
		check = true;
	}
	if (startAttack == true && check == true)
		if (GetTickCount() - wait > 3000)
			this->SetState(BOSSBAT_STATE_ATTACK);
}

void BossBat::Render()
{
	int ani;
	

	if (state == BOSSBAT_STATE_WAIT)
		ani = BOSSBAT_ANI_WAIT;
	else if (state == BOSSBAT_STATE_ATTACK)
		ani = BOSSBAT_ANI_ATTACK;
	animations[ani]->Render(x, y);
	
	RenderBoundingBox();
}

void BossBat::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case BOSSBAT_STATE_WAIT:
			vx = vy = 0;
			break;
		case BOSSBAT_STATE_ATTACK:
			
			break;

	}

}