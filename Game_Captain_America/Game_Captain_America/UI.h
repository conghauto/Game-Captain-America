#pragma once
#include "d3dx9.h"
#include "define.h"
#include <string>
#include "Sprites.h"
#include <vector>
#include "Simon.h"
#include "define.h"
#include "Textures.h"
#include "Sprites.h"
class UI
{
private:
	ID3DXFont *font;
	RECT rect;
	Simon *simon;

	std::string information;

	std::vector<LPSPRITE> simonHPList;
	std::vector<LPSPRITE> enemyHPList;
	std::vector<LPSPRITE> noHPList;

	CSprite* heart;
	CSprite* weapon;
	CSprite* pinkrect;
	
	CSprite* holyWater;
	CSprite* cross;
	CSprite* knife;
	CSprite* stopWatch;

	CSprite* upgrade1;
	CSprite* upgrade2;

	int simonHP;
	int bossHP;
	int uiHP;

	int score;
	int time;
	int item;
	int energy;
	int life;
	int stage;
	int subWeapon;

public:
	UI();
	~UI();
	bool Initialize(LPDIRECT3DDEVICE9 d3ddv,Simon *simon);
	//void Update(int bossHP, int time, int life, int stage);
	void Update( int time, int stage,Simon *simon);
	void Render(float x,float y,Simon *simon);
	CSprite* axe;
};

