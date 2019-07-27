#include <algorithm>
#include "debug.h"

#include "Captain.h"
#include "Game.h"
#include "Item.h"
#include "Zombie.h"
#include "Ground.h"
#include "Lantern.h"
#include "CheckPoint.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int Captain::score = 0;
int Captain::heartsAmount = 5;
/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Captain::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	bool isCollideWithCheckBox = false;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Captain se khong va cham voi nhung vat sau:
		if (!dynamic_cast<Lantern *>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Captain::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Has completed attack animation
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isAttack = false;
		if (isExitSit)
		{
			isSit = false;
			y -= SIMON_SIT_TO_STAND;
			isExitSit = false;
		}
		// Check collision between whip and game objects here
		whip->Update(dt, coObjects);
	}

	// Simple fall down
		vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// turn off isOnCheckStair


	// Set idle state
	if (!isSit && !isMoving && !isJump && !isAttack)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (untouchable != 0 && GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	// Handle Captain go over screen camera
	float leftCorner = CGame::GetInstance()->x_cam;
	float rightCorner = leftCorner + SCREEN_WIDTH - SIMON_STAND_BBOX_WIDTH;
	// Left corner
	if (x < leftCorner)
	{
		x = 0;
	}
	// Right corner
	else if (x > rightCorner)
	{
		//x = rightCorner;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0 )
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		/*
		 * Handle collision here
		 */
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Zombie *>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
				if (zombie->GetState() != ZOMBIE_STATE_DIE) {
					StartUntouchable();
					//SetState(SIMON_STATE_DIE);
				}
			}
			else if (dynamic_cast<Ground *>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (isJump && e->nx == 0 && e->ny < 0)
					isJump = false;

				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<CheckPoint *>(e->obj))
			{
				CheckPoint *checkpoint = dynamic_cast<CheckPoint *>(e->obj);
				int type = checkpoint->GetType();

				switch (type)
				{
				case CHECKPOINT_BONUS:
					isBonus = true;
					checkpoint->SetType(CHECKPOINT_BONUS);
					break;
				/*case CHECKPOINT_LEVELUP:
					if (!isLevelUp)
					{
						x -= nx * 1.0f;
						isLevelUp = true;
						onCheckPointTime = GetTickCount();
					}
					break;*/
				}
			}
			else if (dynamic_cast<Item *>(e->obj))
			{
				Item *item = dynamic_cast<Item *>(e->obj);
				item->SetEaten();

				int type = item->GetType();
				switch (type)
				{
				case ITEM_HEART:
					IncHeart(5);
					break;
				case ITEM_WHIPITEM:
					whip->UpLevel();
					break;
				case ITEM_KNIFE:
					SetCurrentWeapon(ITEM_KNIFE);
					break;
				case ITEM_MONEY:
					IncScore(1000);
					break;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Captain::Render()
{
	int ani = -1, aniWhip = -1;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	{
		if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_RIGHT;
				}
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_LEFT;
				}
			}
		}
		else if (isJump)
		{
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		else if (isDoubleJump) {
			ani = SIMON_ANI_DOUBLE_JUMP;
			isJump = false;
		}
		// Right direction
		else if (nx > 0)
		{
			switch (state)
			{
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_SIT_RIGHT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_DASHING:
				ani = SIMON_ANI_DASHING_RIGHT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_RIGHT;
				break;
			}
		}
		// Left direction
		else if (nx < 0)
		{
			switch (state)
			{
			case SIMON_STATE_ONCHECKSTAIR:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				else
					ani = SIMON_ANI_SIT_LEFT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_DASHING:
				ani = SIMON_ANI_DASHING_LEFT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_LEFT;
				break;
			}
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	if (ani != -1)
	{
		switch (ani)
		{
		case SIMON_ANI_ATTACK_RIGHT:
		case SIMON_ANI_SIT_ATTACK_RIGHT:
			animations[ani]->Render(x - 16, y, alpha);
			break;
		case SIMON_ANI_HITUP_RLADDER:
			animations[ani]->Render(x - 18, y, alpha);
			break;
		case SIMON_ANI_ATTACK_LEFT:
		case SIMON_ANI_SIT_ATTACK_LEFT:
			animations[ani]->Render(x - 12, y, alpha);
			break;
		default:
			animations[ani]->Render(x, y, alpha);
			break;
		}
	}

	RenderBoundingBox();
}

void Captain::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_DIE:
		isDead = true;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;

	case SIMON_STATE_SIT:
		//isOnCheckStair = false;
		isSit = true;
		vx = 0;
		break;
	case SIMON_STATE_WALK:
		//isOnCheckStair = false;
		isMoving = true;
		if (nx == 1.0f)
		{
			vx = SIMON_WALKING_SPEED;
		}
		else if (nx == -1.0f)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_IDLE:
		//isOnCheckStair = false;
		vx = 0;
		isMoving = false;
		break;
	case SIMON_STATE_DASHING:
		isMoving = true;
		if (nx == 1.0f)
		{
			vx = 0.2f;
		}
		else if (nx == -1.0f)
		{
			vx = -0.2f;
		}
		isDashing = true;
		break;
	case SIMON_STATE_DOUBLE_JUMP:
		isDoubleJump = true;
		vy = -0.6f;
		break;
	}
}

void Captain::SetAction(int action)
{
	switch (action)
	{
	case SIMON_ACTION_ATTACK:
		// Ngat tat ca trang thai (tru ngoi)
		// Neu dang nhay toi thi de nguyen van toc, neu khong thi dung lai va danh
		isAttack = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;
		// Don gian la cho nhay, khong ngat bat ki trang thai nao
		// Them van toc nhay
	case SIMON_ACTION_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void Captain::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SIMON_STAND_BBOX_WIDTH;
	bottom = y + SIMON_STAND_BBOX_HEIGHT;

}


