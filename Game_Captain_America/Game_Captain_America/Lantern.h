#include"GameObject.h"
#include"Effect.h"

class Lantern :public CGameObject
{
public:
	bool isHitted;
	Lantern() :CGameObject() {
		isHitted = false;
	}
	void SetShoot();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

