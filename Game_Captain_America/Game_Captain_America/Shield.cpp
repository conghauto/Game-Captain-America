#include "Shield.h"
#include "Lantern.h"
#include "Soldier.h"

void Shield::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Outrange
		if (nx > 0)
		{
			if (this->x > this->max_x)
				isEaten = true;
		}
		else if (nx < 0)
		{
			if (this->x < this->max_x)
				isEaten = true;
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		x += dx;
		y += dy;

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{

		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Zombie *>(e->obj))
				{
					Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
					zombie->SetState(ZOMBIE_STATE_DIE);
					this->isEaten = true;
				}
				else if (dynamic_cast<Soldier *>(e->obj))
				{
					Soldier *soldier = dynamic_cast<Soldier *>(e->obj);
					soldier->SetState(SOLDIER_STATE_DIE);
					this->isEaten = true;
				}
				else if (dynamic_cast<Lantern *>(e->obj))
				{
					Lantern *lantern = dynamic_cast<Lantern *>(e->obj);
					lantern->isHitted = true;
					this->isEaten = true;
				}
			}
		}
	}
}