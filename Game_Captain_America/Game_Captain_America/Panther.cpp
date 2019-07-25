#include "Panther.h"
#include "define.h"

void Panther::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + PANTHER_BBOX_WIDTH;

	if (state == PANTHER_STATE_DIE)
		bottom = y + PANTHER_BBOX_HEIGHT_DIE;
	else
		bottom = y + PANTHER_BBOX_HEIGHT;
}

void Panther::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
	/*x += dx;
	y += dy;*/
	if (state == PANTHER_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x >MAX_WIDTH_LV2) {
		x = MAX_WIDTH_LV2; vx = -vx;
	}
	/*float check = this->x - (CGame::GetInstance()->x_cam + SCREEN_WIDTH / 2);
	if (check < 100 )
		this->SetState(PANTHER_STATE_ATTACK);*/
	 

}

void Panther::Render()
{
	int ani;
	if (state == PANTHER_STATE_DIE) {
		ani = PANTHER_ANI_DIE;
		return;
	}
	if (state == PANTHER_STATE_WAIT) {
		ani = PANTHER_ANI_WAIT;
		
	}
	else if(state == PANTHER_STATE_ATTACK) {		
		if (vx < 0)
			ani = PANTHER_ANI_RUN_LEFT;
		else if (vx > 0)
			ani = PANTHER_ANI_RUN_RIGHT;			
	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Panther::SetState(int state)
{
	
	CGameObject::SetState(state);
	switch (state)
	{
	case PANTHER_STATE_WAIT:
		vx = 0;
		vy = 0;
		break;
	case PANTHER_STATE_ATTACK:		
		vx = -PANTHER_WALKING_SPEED;				
	}
}
