#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Simon.h"
#include "Ground.h"
#include "define.h"
#include "Zombie.h"
#include "BigFire.h"
#include "Candle.h"
#include "Item.h"
#include "Lantern.h"
#include "Map.h"
#include "Stair.h"
#include "Panther.h"
#include "CheckStair.h"
#include "CheckPoint.h"
#include "UI.h"
#include "BossBat.h"

CGame *game;
Simon * simon;
Item *item;
Effect *whipEffect;
Map *map;
//UI * ui;
CSprite *sprite;
vector<LPGAMEOBJECT> objects;
vector<int> willDeleteObjects;
bool lv1 = true;
bool lv2 = false;
bool lv2_1 = false;
bool lv2_2 = false;
bool boss = false;
// check scene lv2->lv2_1
bool checkScene = false;
bool check1 = false;
bool check = false;
//
//check scene lv2_1 ->lv2_2
bool checkScene1 = false;
bool check3 = false;
bool countLoadResourceLv2 = false;
bool countLoadResourceLv2_1 = false;
bool countLoadResourceLv2_2 = false;
bool countLoadResourceboss = false;
DWORD timer; // load enemy lv2
DWORD timer2;//load enemy boss
DWORD gameTime = 999000;
CSprites * sprites = CSprites::GetInstance();
CAnimations * animations = CAnimations::GetInstance();
CTextures * textures = CTextures::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Nhay
	if (KeyCode == DIK_Z)
	{
		if (simon->isJump == false && simon->isSit == false && simon->isAttack == false && simon->isOnStair == false)
			simon->SetAction(SIMON_ACTION_JUMP);
	}

	if (KeyCode == DIK_C)
	{
		if (simon->isDoubleJump == false && simon->isSit == false && simon->isAttack == false && simon->isOnStair == false)
			simon->SetState(SIMON_STATE_DOUBLE_JUMP);
	}
	// Danh
	if (KeyCode == DIK_X)
	{
		if (simon->isAttack == false)
			simon->SetAction(SIMON_ACTION_ATTACK);
	}

}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (KeyCode == DIK_UP)
	{
		if (simon->isHoldShield)
		{
			simon->SetState(SIMON_STATE_IDLE);
			simon->isHoldShield = false;
		}
		else if (simon->isOnCheckStairUp)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
		else if (simon->isOnStair)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
	}

	if (KeyCode == DIK_Q) {
		simon->SetState(SIMON_STATE_IDLE);
		simon->isDashing = false;

	}
	if (KeyCode == DIK_C) {
		simon->SetState(SIMON_STATE_IDLE);
		simon->isDoubleJump = false;
		simon->isJump = false;
	}
	// Ngoi
	if (KeyCode == DIK_DOWN)
	{
		if (simon->isOnCheckStairDown)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
		else if (simon->isOnStair)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (simon->isSit)
		{
			if (!simon->isAttack)
			{
				simon->isSit = false;
			}
			else
			{
				simon->isExitSit = true;
			}
		}
	}

	// Di bo
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
	{
		simon->isMoving = false;
		simon->vx = 0;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (game->IsKeyDown(DIK_UP))
	{
		//if (!simon->isOnCheckStairDown && !simon->isOnStair && !simon->isAttack && !simon->isJump)
		simon->SetState(SIMON_STATE_HOLD_SHIELD);
		//if (simon->ny == -1 && !simon->isOnStair)
		//{
		//	if (!simon->isAttack && !simon->isSit && !simon->isJump)
		//	{
		//		simon->SetState(SIMON_STATE_ONCHECKSTAIR);
		//	}
		//}
		//else if (simon->isOnStair && !simon->isAttack)
		//{
		//	simon->ny = -1;
		//	simon->SetState(SIMON_STATE_ONSTAIR);
		//	if (simon->isLeftToRight)
		//		simon->nx = 1;
		//	else
		//		simon->nx = -1;
		//}
		//else if (simon->isOnStair && simon->isAttack)
		//{
		//	simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		//}
	}

	// Ngoi
	if (game->IsKeyDown(DIK_Q)) {
		simon->SetState(SIMON_STATE_DASHING);
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->ny == 1 && !simon->isOnStair)
		{
			if (!simon->isAttack && !simon->isSit && !simon->isJump)
			{
				simon->SetState(SIMON_STATE_ONCHECKSTAIR);
			}
		}
		else if (simon->isOnStair && !simon->isAttack)
		{
			simon->ny = 1;
			simon->SetState(SIMON_STATE_ONSTAIR);
			if (simon->isLeftToRight)
				simon->nx = -1;
			else
				simon->nx = 1;
		}
		else if (simon->isOnStair && simon->isAttack)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (!simon->isOnCheckStairDown && !simon->isOnStair && !simon->isAttack && !simon->isJump)
			simon->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
			simon->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
			simon->nx = -1.0f;
	}

	// Neu khong co gi xay ra se dung im
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	textures->Add(ID_TEX_MARIO, L"Resources\\captian.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_MARIO_2, L"Resources\\american.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_LV1, L"Resources\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Resources\\maplv2.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BRICK, L"Resources\\2.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_BRICK2, L"Resources\\BRICK1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ZOMBIE, L"Resources\\ZOMBIE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ZOMBIE_RIGHT, L"Resources\\ZOMBIE_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER, L"Resources\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER_RIGHT, L"Resources\\PANTHER_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_FIRE, L"Resources\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Resources\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Resources\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Resources\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"Resources\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET_2, L"Resources\\tileset2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET_3, L"Resources\\tileset3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Resources\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER, L"Resources\\3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER_LEFT, L"Resources\\3_.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_STAIR_BOT, L"Resources\\stair_bottom.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_STAIR_TOP, L"Resources\\stair_top.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_MERMAN_LEFT, L"Resources\\MERMAN.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MERMAN_RIGHT, L"Resources\\MERMAN_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BAT, L"Resources\\BAT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BOSS, L"Resources\\VAMPIRE_BAT.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_AXE, L"Resources\\UI\\AXE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BLACK, L"Resources\\UI\\black.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CROSS, L"Resources\\UI\\CROSS.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DOUBLE_SHOT_UI, L"Resources\\UI\\DOUBLE_SHOT_UI.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DOUBLE_SHOT, L"Resources\\UI\\DOUBLE_SHOT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TRIPLE_SHOT_UI, L"Resources\\UI\\TRIPLE_SHOT_UI.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TRIPLE_SHOT, L"Resources\\UI\\TRIPLE_SHOT.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_ENEMYHP, L"Resources\\UI\\EnemyHP.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_HEART, L"Resources\\SMALL_HEART.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HOLY_WATER, L"Resources\\UI\\HOLY_WATER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HP, L"Resources\\UI\\HP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_KNIFE, L"Resources\\UI\\KNIFE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_NOHP, L"Resources\\UI\\NoHP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_STOP_WATCH, L"Resources\\UI\\STOP_WATCH.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_RECT, L"Resources\\rect.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LANTERN, L"Resources\\lantern.png", D3DCOLOR_XRGB(255,0,255));


	textures->Add(ID_TEX_EFFECT1, L"Resources\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT2, L"Resources\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_VIP, L"Resources\\WHIP_VIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONEY, L"Resources\\money_bag_red.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIT, L"Resources\\sitstate.png", D3DCOLOR_XRGB(173, 214, 214));



	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_MARIO);
	LPDIRECT3DTEXTURE9 texSimonSit = textures->Get(ID_TEX_SIT);
	#pragma region Addsprite


	sprites->Add(10001, 232, 3, 258, 45, texSimon);		// đứng im phải

	sprites->Add(10002, 233, 52, 260, 94, texSimon);		// đi phải
	sprites->Add(10003, 260, 51, 286, 94, texSimon);
	sprites->Add(11002, 233, 52, 260, 94, texSimon);
	sprites->Add(11003, 260, 51, 286, 94, texSimon);

	sprites->Add(10004, 231, 138, 273, 178, texSimon);		// đánh phải			
	sprites->Add(10005, 273, 138, 315, 178, texSimon);
	//sprites->Add(10006, 0, 0, 64, 64, texSimon);

	sprites->Add(10028, 259, 264, 297, 311, texSimonSit);		// ngồi đánh phải			
	sprites->Add(10029, 299, 263, 328, 310, texSimonSit);
	//sprites->Add(10030, 360, 134, 421, 197, texSimon);

	//sprites->Add(10034, 120, 134, 168, 197, texSimon);		// trên thang phải đánh phải			
	//sprites->Add(10035, 60, 134, 109, 197, texSimon);
	//sprites->Add(10036, 0, 134, 59, 197, texSimon);

	//sprites->Add(10040, 302, 134, 350, 197, texSimon);		// trên thang trái đánh phải			
	//sprites->Add(10041, 242, 134, 290, 197, texSimon);
	//sprites->Add(10042, 182, 134, 239, 197, texSimon);

	//sprites->Add(10046, 133, 67, 165, 130, texSimon); // lên thang phải
	//sprites->Add(10047, 196, 67, 228, 130, texSimon);

	//sprites->Add(10052, 133, 67, 165, 130, texSimon); //xuống thang phải
	//sprites->Add(10053, 318, 67, 350, 130, texSimon);


	sprites->Add(10018, 285, 146, 311, 191, texSimonSit); // ngồi phải

	sprites->Add(10020, 233, 96, 255, 135, texSimon); // nhảy phải

	sprites->Add(10054, 436, 67, 471, 130, texSimon);//bị đánh từ bên trái

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_MARIO);
	sprites->Add(10011, 203, 3, 227, 48, texSimon2);		// đứng im trái

	sprites->Add(10012, 203, 52, 230, 94, texSimon2);		// đi trái
	sprites->Add(10013, 175, 51, 199, 94, texSimon2);
	sprites->Add(11012, 203, 52, 230, 94, texSimon2);
	sprites->Add(11013, 175, 51, 199, 94, texSimon2);

	sprites->Add(10014, 189, 138, 229, 177, texSimon2);		// đánh trái				
	sprites->Add(10015, 146, 138, 187, 177, texSimon2);
	//sprites->Add(10016, 421, 0, 464, 64, texSimon2);

	sprites->Add(10031, 216, 265, 257, 310, texSimonSit);		// ngồi đánh trái				
	sprites->Add(10032, 185, 265, 217, 310, texSimonSit);
	//sprites->Add(10033, 60, 134, 104, 197, texSimon2);

	//sprites->Add(10037, 120, 134, 177, 197, texSimon2);		// trên thang phải đánh trái			
	//sprites->Add(10038, 180, 134, 225, 197, texSimon2);
	//sprites->Add(10039, 240, 134, 282, 197, texSimon2);

	//sprites->Add(10043, 300, 134, 361, 197, texSimon2);		// trên thang trái đánh trái			
	//sprites->Add(10044, 360, 134, 405, 197, texSimon2);
	//sprites->Add(10045, 420, 134, 464, 197, texSimon2);


	//sprites->Add(10048, 190, 67, 224, 130, texSimon2); // xuống thang trái
	//sprites->Add(10049, 129, 67, 163, 130, texSimon2);

	//sprites->Add(10050, 190, 67, 224, 130, texSimon2); //lên thang trái
	//sprites->Add(10051, 252, 67, 286, 130, texSimon2);

	sprites->Add(10019, 253, 147, 281, 191, texSimonSit); // ngồi trái

	sprites->Add(10021, 203, 97, 229, 136, texSimon2);//nhảy trái

	sprites->Add(10055, 12, 70, 41, 130, texSimon2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 

	sprites->Add(10110, 106, 144, 149, 189, texSimonSit);//nhay danh phai

	sprites->Add(10111, 59, 144, 102, 189, texSimonSit); // nhay danh trai

	sprites->Add(10120, 282, 96, 302, 136, texSimon); // double nhay 
	sprites->Add(10121, 305, 96, 327, 136, texSimon);
	sprites->Add(10122, 327, 96, 349, 136, texSimon);

	sprites->Add(10130, 237, 79, 276, 123, texSimonSit);// dashing left
	sprites->Add(10131, 310, 79, 348, 123, texSimonSit);// dashing right

	sprites->Add(10140, 233, 224, 258, 268, texSimon); // shielding right
	sprites->Add(10141, 202, 224, 227, 268, texSimon);	//shielding left

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);
	sprites->Add(10022, 570, 200, 554, 267, whipR);			//roi lv2 phải
	sprites->Add(10023, 342, 260, 312, 316, whipR);
	sprites->Add(10024, 342, 328, 458, 364, whipR); // 116

	sprites->Add(10062, 570, 200, 554, 267, whipR);			//roi lv1 phải	
	sprites->Add(10063, 342, 260, 312, 316, whipR);
	sprites->Add(10064, 112, 65, 189, 100, whipR); 
	
	sprites->Add(10056, 570, 0, 553, 67, whipR);			//roi lv0 phải	
	sprites->Add(10057, 346, 0, 312, 49, whipR);
	sprites->Add(10058, 114, 0, 188, 30, whipR); // 74

	
	

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);
	sprites->Add(10025, 105, 70, 165, 133, whipL);			//roi lv2 trái
	sprites->Add(10026, 340, 65, 406, 117, whipL);
	sprites->Add(10027, 575, 330, 458, 364, whipL);

	sprites->Add(10065, 105, 70, 165, 133, whipL);			//roi lv1 trái	
	sprites->Add(10066, 340, 65, 406, 117, whipL);
	sprites->Add(10067, 572, 65, 530, 100, whipL); 

	sprites->Add(10059, 110, 0, 166, 66, whipL);			//roi lv0 trái
	sprites->Add(10060, 349, 0, 408, 52, whipL);
	sprites->Add(10061, 585, 0, 529, 30, whipL);

	


	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 32, 32, texMisc);

	LPDIRECT3DTEXTURE9 texMisc2 = textures->Get(ID_TEX_BRICK2);
	sprites->Add(20002, 0, 0, 32, 32, texMisc2);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	sprites->Add(30002, 36, 0, 68, 64, texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add(30003, 0, 0, 32, 64, texEnemy0); // đi Phải 
	sprites->Add(30004, 36, 0, 68, 64, texEnemy0);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 50, 30, texEnemy1); // báo
	sprites->Add(30012, 64, 0, 120, 30, texEnemy1); // báo chạy trái
	sprites->Add(30013, 126, 0, 188, 30, texEnemy1);
	sprites->Add(30014, 188, 0, 256, 30, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy10 = textures->Get(ID_TEX_PANTHER_RIGHT);	
	sprites->Add(30015, 136, 0, 190, 30, texEnemy10); //báo  chạy phải
	sprites->Add(30016, 67, 0, 130, 30, texEnemy10);
	sprites->Add(30017, 0, 0, 64, 30, texEnemy10);

	LPDIRECT3DTEXTURE9 texEnemy11 = textures->Get(ID_TEX_BAT);
	sprites->Add(30018, 0, 0, 32, 32, texEnemy11); // dơi // nằm chờ
	sprites->Add(30019, 32, 0, 64, 32, texEnemy11);
	sprites->Add(30020, 64, 0, 96, 32, texEnemy11);
	sprites->Add(30021, 96, 0, 128, 32, texEnemy11);

	LPDIRECT3DTEXTURE9 texEnemy12 = textures->Get(ID_TEX_BOSS);
	sprites->Add(30022, 27, 0, 67, 38, texEnemy12); // boss // nằm chờ
	sprites->Add(30023, 98, 0, 200, 45, texEnemy12);
	sprites->Add(30024, 213, 0, 281, 45, texEnemy12);
	
	LPDIRECT3DTEXTURE9 texEnemy13 = textures->Get(ID_TEX_MERMAN_LEFT);
	sprites->Add(30025, 0, 0, 32, 64, texEnemy13); //người cá trái
	sprites->Add(30026, 32, 0, 64, 64, texEnemy13);
	sprites->Add(30027, 64, 0, 96, 64, texEnemy13);

	LPDIRECT3DTEXTURE9 texEnemy14 = textures->Get(ID_TEX_MERMAN_RIGHT);
	sprites->Add(30028, 64, 0, 96, 64, texEnemy14); // người cá phải
	sprites->Add(30029, 32, 0, 64, 64, texEnemy14);
	sprites->Add(30030, 0, 0, 32, 64, texEnemy14);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);

	LPDIRECT3DTEXTURE9 texEnemy3 = textures->Get(ID_TEX_CANDLE);
	sprites->Add(40013, 0, 0, 16, 32, texEnemy3);
	sprites->Add(40014, 16, 0, 32, 32, texEnemy3);

	LPDIRECT3DTEXTURE9 texMisc3 = textures->Get(ID_TEX_LADDER);
	sprites->Add(40015, 0, 0, 32, 32, texMisc3);

	LPDIRECT3DTEXTURE9 texMisc4 = textures->Get(ID_TEX_LADDER_LEFT);
	sprites->Add(40016, 0, 0, 32, 32, texMisc4);

	LPDIRECT3DTEXTURE9 texMisc5 = textures->Get(ID_TEX_STAIR_BOT);
	sprites->Add(40017, 0, 0, 32, 32, texMisc5);

	LPDIRECT3DTEXTURE9 texMisc6 = textures->Get(ID_TEX_STAIR_TOP);
	sprites->Add(40018, 0, 0, 32, 32, texMisc6);

	LPDIRECT3DTEXTURE9 texMisc7 = textures->Get(ID_TEX_HEART);
	sprites->Add(40019, 0, 0, 16, 16, texMisc7);

	LPDIRECT3DTEXTURE9 texMisc8 = textures->Get(ID_TEX_EFFECT1);
	sprites->Add(40020, 0, 0, 16, 20, texMisc8);

	LPDIRECT3DTEXTURE9 texMisc9 = textures->Get(ID_TEX_EFFECT2);
	sprites->Add(40021, 0, 10, 42, 37, texMisc9);
	sprites->Add(41021, 42, 10, 84, 37, texMisc9);
	sprites->Add(42021, 84, 10, 126, 37, texMisc9);

	LPDIRECT3DTEXTURE9 texMisc10 = textures->Get(ID_TEX_WHIP_VIP);
	sprites->Add(40022, 0, 0, 32, 32, texMisc10);

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE);
	sprites->Add(40023, 0, 0, 32, 18, texMisc11);

	LPDIRECT3DTEXTURE9 texMisc12 = textures->Get(ID_TEX_MONEY);
	sprites->Add(40024, 0, 0, 30, 30, texMisc12);

	LPDIRECT3DTEXTURE9 texMisc13 = textures->Get(ID_TEX_LANTERN);
	sprites->Add(40025, 0, 0, 16, 16, texMisc13);
	#pragma endregion

	LPANIMATION ani;

	#pragma region SimonAnimation

	ani = new CAnimation(100);	//đứng phải
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	//đứng trái
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(180);	//đi phải
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(11002);
	ani->Add(11003);
	animations->Add(500, ani);

	ani = new CAnimation(180);	//đi trái
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(11012);
	ani->Add(11013);
	animations->Add(501, ani);

	ani = new CAnimation(150); //đánh phải
	ani->Add(10004);
	ani->Add(10005);
	//ani->Add(10006);

	animations->Add(402, ani);

	ani = new CAnimation(150);//đánh trái
	ani->Add(10014);
	ani->Add(10015);
	//ani->Add(10016);

	animations->Add(403, ani);

	ani = new CAnimation(100); //ngồi phải
	ani->Add(10018);
	animations->Add(404, ani);

	ani = new CAnimation(100);//ngồi trái
	ani->Add(10019);
	animations->Add(405, ani);

	ani = new CAnimation(100);//nhảy phải
	ani->Add(10020);
	animations->Add(406, ani);

	ani = new CAnimation(100);//nhảy trái
	ani->Add(10021);
	animations->Add(407, ani);

	ani = new CAnimation(150);//roi phải 
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	animations->Add(408, ani);

	ani = new CAnimation(150);//roi trái
	ani->Add(10025);
	ani->Add(10026);
	ani->Add(10027);
	animations->Add(409, ani);

	ani = new CAnimation(150);//ngồi đánh phải
	ani->Add(10028);
	ani->Add(10029);
	//ani->Add(10030);
	animations->Add(410, ani);

	ani = new CAnimation(150);//ngồi đánh trái
	ani->Add(10031);
	ani->Add(10032);
	//ani->Add(10033);
	animations->Add(411, ani);

	ani = new CAnimation(150);//trên thang phải đánh phải
	ani->Add(10034);
	ani->Add(10035);
	ani->Add(10036);
	animations->Add(412, ani);

	ani = new CAnimation(150);//trên thang phải đánh trái
	ani->Add(10037);
	ani->Add(10038);
	ani->Add(10039);
	animations->Add(413, ani);

	ani = new CAnimation(150);//trên thang trái đánh phải
	ani->Add(10040);
	ani->Add(10041);
	ani->Add(10042);
	animations->Add(414, ani);

	ani = new CAnimation(150);//trên thang trái đánh trái
	ani->Add(10043);
	ani->Add(10044);
	ani->Add(10045);
	animations->Add(415, ani);


	ani = new CAnimation(100);//lên thang phải
	ani->Add(10046);
	ani->Add(10047);
	animations->Add(416, ani);

	ani = new CAnimation(100);//xuống thang phải
	ani->Add(10048);
	ani->Add(10049);
	animations->Add(417, ani);

	ani = new CAnimation(100);//lên thang trái
	ani->Add(10050);
	ani->Add(10051);
	animations->Add(418, ani);

	ani = new CAnimation(100);//xuống thang trái
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(419, ani);

	ani = new CAnimation(100);//đứng im bên phải thang phải
	ani->Add(10047);
	animations->Add(420, ani);

	ani = new CAnimation(100);//đứng im bên phải thang trái
	ani->Add(10049);
	animations->Add(421, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10051);
	animations->Add(422, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10053);
	animations->Add(423, ani);

	ani = new CAnimation(100);//bị đánh từ bên trái
	ani->Add(10054);	
	animations->Add(424, ani);

	ani = new CAnimation(100);//bị đánh từ bên phải
	ani->Add(10055);	
	animations->Add(425, ani);

	ani = new CAnimation(150);//roi lv0 phải
	ani->Add(10056);
	ani->Add(10057);
	ani->Add(10058);
	animations->Add(426, ani);

	ani = new CAnimation(150);//roi lv0 trái
	ani->Add(10059);
	ani->Add(10060);
	ani->Add(10061);
	animations->Add(427, ani);

	ani = new CAnimation(150);//roi lv1 phải
	ani->Add(10062);
	ani->Add(10063);
	ani->Add(10064);
	animations->Add(428, ani);

	ani = new CAnimation(150);//roi lv1 trái
	ani->Add(10065);
	ani->Add(10066);
	ani->Add(10067);
	animations->Add(429, ani);

	ani = new CAnimation(100);	//chết	
	ani->Add(10099);
	animations->Add(599, ani);


	ani = new CAnimation(100); //nhay danh phai
	ani->Add(10110);
	animations->Add(1600, ani);

	ani = new CAnimation(100); //nhay danh trai
	ani->Add(10111);
	animations->Add(1601, ani);

	ani = new CAnimation(100); //double nhay
	ani->Add(10120);
	ani->Add(10121);
	ani->Add(10122);
	animations->Add(1602, ani);

	ani = new CAnimation(100); //dashing right
	ani->Add(10131);
	animations->Add(1603, ani);

	ani = new CAnimation(100); //dashing left
	ani->Add(10130);
	animations->Add(1604, ani);
		

	ani = new CAnimation(100); //shielding right
	ani->Add(10140);
	animations->Add(1605, ani);

	ani = new CAnimation(100); //shielding left
	ani->Add(10141);
	animations->Add(1606, ani);

	#pragma endregion

	#pragma region ObjectAnimation

	ani = new CAnimation(100);	//đất1
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100); //zombie đi trái
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(602, ani);

	ani = new CAnimation(100); //zombie đi phải
	ani->Add(30003);
	ani->Add(30004);
	animations->Add(604, ani);

	ani = new CAnimation(100);	//đất2
	ani->Add(20002);
	animations->Add(603, ani);

	ani = new CAnimation(100); //panther nằm chờ
	ani->Add(30011);
	animations->Add(605, ani);

	ani = new CAnimation(100); //panther chạy trái
	ani->Add(30012);
	ani->Add(30013);
	ani->Add(30014);
	animations->Add(606, ani);

	ani = new CAnimation(100); //panther chạy phải
	ani->Add(30015);
	ani->Add(30016);
	ani->Add(30017);
	animations->Add(607, ani);

	ani = new CAnimation(100); //panther phóng
	ani->Add(30014);
	animations->Add(608, ani);

	ani = new CAnimation(100); //boss dơi nằm chờ
	ani->Add(30022);
	animations->Add(609, ani);

	ani = new CAnimation(100); //boosss dơi bay
	ani->Add(30023);
	ani->Add(30024);
	animations->Add(610, ani);

	ani = new CAnimation(100); //fire
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);

	ani = new CAnimation(100); //candle
	ani->Add(40013);
	ani->Add(40014);
	animations->Add(800, ani);

	ani = new CAnimation(0); //STAIR RIHGT
	ani->Add(40015);
	animations->Add(801, ani);

	ani = new CAnimation(0); //STAIR LEFT
	ani->Add(40016);
	animations->Add(802, ani);

	ani = new CAnimation(0); //STAIR BOT
	ani->Add(40017);
	animations->Add(803, ani);

	ani = new CAnimation(0); //STAIR TOP
	ani->Add(40018);
	animations->Add(804, ani);

	ani = new CAnimation(0); //heart
	ani->Add(40019);
	animations->Add(805, ani);

	ani = new CAnimation(150); //hieu ung' dau' sao
	ani->Add(40020);
	animations->Add(806, ani);

	ani = new CAnimation(50); //hieu ung toe' lua
	ani->Add(40021);
	ani->Add(41021);
	ani->Add(42021);
	animations->Add(807, ani);

	ani = new CAnimation(0); //whip item nang cap
	ani->Add(40022);
	animations->Add(808, ani);

	ani = new CAnimation(0); //knife
	ani->Add(40023);
	animations->Add(809, ani);

	ani = new CAnimation(0); //money
	ani->Add(40024);
	animations->Add(810, ani);

	ani = new CAnimation(0); //đèn lồng
	ani->Add(40025);
	animations->Add(811, ani);

	#pragma endregion

	#pragma region simon
	simon = new Simon();
	simon->AddAnimation(400);	// đứng phải	
	simon->AddAnimation(401);	//đứng trái	
	simon->AddAnimation(500);	// đi phải
	simon->AddAnimation(501);	//đi trái
	simon->AddAnimation(402);	//đánh phải 
	simon->AddAnimation(403);	//đánh trái
	simon->AddAnimation(404);	//ngồi phải
	simon->AddAnimation(405);	//ngồi trái
	simon->AddAnimation(406);	//nhảy phải 
	simon->AddAnimation(407);	//nhảy trái
	simon->AddAnimation(410);	//ngồi đánh phải 
	simon->AddAnimation(411);	//ngồi đánh trái
	simon->AddAnimation(412);	//trên thang phải đánh phải
	simon->AddAnimation(413);	//trên thang phải đánh trái
	simon->AddAnimation(414);	//trên thang trái đánh phải
	simon->AddAnimation(415);	//trên thang trái đánh trái
	simon->AddAnimation(416);	//lên thang phải
	simon->AddAnimation(417);	//xuống thang phải
	simon->AddAnimation(418);	//lên thang trái
	simon->AddAnimation(419);	//xuống thang trái
	simon->AddAnimation(420);	//đứng im bên phải thang phải
	simon->AddAnimation(421);	//đứng im bên phải thang trái
	simon->AddAnimation(422);	//đứng im bên trái thang phải
	simon->AddAnimation(423);	//đứng im bên trái thang trái
	simon->AddAnimation(424);	//bị đánh từ bên trái
	simon->AddAnimation(425);	//bị đánh từ bên phải
	simon->AddAnimation(599);	//chết
	simon->AddAnimation(1600); // nhay danh phai
	simon->AddAnimation(1601); // nhay danh trai
	simon->AddAnimation(1602); // double nhay
	simon->AddAnimation(1603); // dashing right
	simon->AddAnimation(1604); // dashing left
	simon->AddAnimation(1605);	//shielding right
	simon->AddAnimation(1606);	//shielding left

	
	simon->whip->AddAnimation(426);//roi lv0 phải
	simon->whip->AddAnimation(427);//roi lv0 trái
	simon->whip->AddAnimation(428);//roi lv1 phải
	simon->whip->AddAnimation(429);//roi lv1 trái
	simon->whip->AddAnimation(408);//roi lv2 phải
	simon->whip->AddAnimation(409);//roi lv2 trái
	simon->SetPosition(15, 160);
	objects.push_back(simon);

	#pragma endregion

	#pragma region Ground
	for (int i = 0; i < 64; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 32.0f, 245);
		objects.push_back(ground);
	}
	#pragma endregion

	#pragma region BigFire
	BigFire *bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(335, 180);
	objects.push_back(bigfire);

	BigFire *bigfire1 = new BigFire();
	bigfire1->AddAnimation(700);
	bigfire1->SetPosition(464, 180);
	objects.push_back(bigfire1);

	BigFire *bigfire2 = new BigFire();
	bigfire2->AddAnimation(700);
	bigfire2->SetPosition(657, 180);
	objects.push_back(bigfire2);

	BigFire *bigfire3 = new BigFire();
	bigfire3->AddAnimation(700);
	bigfire3->SetPosition(851, 180);
	objects.push_back(bigfire3);

	BigFire *bigfire4 = new BigFire();
	bigfire4->AddAnimation(700);
	bigfire4->SetPosition(1090, 180);
	objects.push_back(bigfire4);

	BigFire *bigfire5 = new BigFire();
	bigfire5->AddAnimation(700);
	bigfire5->SetPosition(1267, 180);
	objects.push_back(bigfire5);

	
#pragma endregion

#pragma region Lantern
	Lantern *lantern1 = new Lantern();
	lantern1->AddAnimation(811);
	lantern1->SetPosition(64,216);
	objects.push_back(lantern1);

	Lantern *lantern2 = new Lantern();
	lantern2->AddAnimation(811);
	lantern2->SetPosition(128, 180);
	objects.push_back(lantern2);

	Lantern *lantern3 = new Lantern();
	lantern3->AddAnimation(811);
	//lantern3->SetPosition(318, 188);
	lantern3->SetPosition(318, 184);
	objects.push_back(lantern3);

	Lantern *lantern4 = new Lantern();
	lantern4->AddAnimation(811);
	lantern4->SetPosition(414, 96);
	objects.push_back(lantern4);

	Lantern *lantern5 = new Lantern();
	lantern5->AddAnimation(811);
	lantern5->SetPosition(574, 128);
	objects.push_back(lantern5);

	Lantern *lantern6 = new Lantern();
	lantern6->AddAnimation(811);
	lantern6->SetPosition(832, 128);
	objects.push_back(lantern6);

	Lantern *lantern7 = new Lantern();
	lantern7->AddAnimation(811);
	lantern7->SetPosition(1088, 180);
	objects.push_back(lantern7);

	Lantern *lantern8 = new Lantern();
	lantern8->AddAnimation(811);
	lantern8->SetPosition(1184, 96);
	objects.push_back(lantern8);

	Lantern *lantern9 = new Lantern();
	lantern9->AddAnimation(811);
	lantern9->SetPosition(1344, 80);
	objects.push_back(lantern9);

	Lantern *lantern10 = new Lantern();
	lantern10->AddAnimation(811);
	lantern10->SetPosition(1408, 172);
	objects.push_back(lantern10);

	Lantern *lantern11 = new Lantern();
	lantern11->AddAnimation(811);
	lantern11->SetPosition(1600, 180);
	objects.push_back(lantern11);

	Lantern *lantern12 = new Lantern();
	lantern12->AddAnimation(811);
	lantern12->SetPosition(1696, 96);
	objects.push_back(lantern12);
#pragma endregion

#pragma region CheckPoint
	CheckPoint *checkPoint;
	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_LEVELUP);
	checkPoint->SetPosition(2048, 374);
	objects.push_back(checkPoint);

	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_BONUS);
	checkPoint->SetPosition(2020, 374);
	objects.push_back(checkPoint);
#pragma endregion

	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	
	/*ui = new UI();
	ui->Initialize(d3ddv,simon);*/
}

void LoadResourceLv2() {
	for (int i = 0; i < 95; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(0 + i * 32.0f, 440);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1377 + i * 32.0f, 310);
		objects.push_back(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1503 + i * 32.0f, 244);
		objects.push_back(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1854 + i * 32.0f, 310);
		objects.push_back(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(2782 + i * 32.0f, 246);
		objects.push_back(ground);
	}
	
	for (int i = 0; i < 5; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(65 + i * 255, 376);
		objects.push_back(candle);
	}
	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(195 + i * 257, 316);
		objects.push_back(candle);
	}
	Candle *candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1219, 326);
	objects.push_back(candle);

	Candle *candle1 = new Candle();
	candle1->AddAnimation(800);
	candle1->SetPosition(1339, 193);
	objects.push_back(candle1);	

	Candle *candle2 = new Candle();
	candle2->AddAnimation(800);
	candle2->SetPosition(1445, 380);
	objects.push_back(candle2);

	Candle *candle3 = new Candle();
	candle3->AddAnimation(800);
	candle3->SetPosition(1800, 380);
	objects.push_back(candle3);

	Candle *candle4 = new Candle();
	candle4->AddAnimation(800);
	candle4->SetPosition(1713, 190);
	objects.push_back(candle4);

	Candle *candle5 = new Candle();
	candle5->AddAnimation(800);
	candle5->SetPosition(1986, 198);
	objects.push_back(candle5);

	Candle *candle6 = new Candle();
	candle6->AddAnimation(800);
	candle6->SetPosition(2610, 387);
	objects.push_back(candle6);

	Candle *candle7 = new Candle();
	candle7->AddAnimation(800);
	candle7->SetPosition(2742, 322);
	objects.push_back(candle7);

	Candle *candle8 = new Candle();
	candle8->AddAnimation(800);
	candle8->SetPosition(2868, 190);
	objects.push_back(candle8);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(2050 +i*125 ,380);
		objects.push_back(candle);
	}
	/*Panther *panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);	
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1403, 260);
	panther->SetState(PANTHER_STATE_WAIT);
	objects.push_back(panther);*/

	Panther *panther1 = new Panther();
	panther1->AddAnimation(605);
	panther1->AddAnimation(606);
	panther1->AddAnimation(607);
	panther1->AddAnimation(608);
	panther1->SetPosition(1700, 200);
	panther1->SetState(PANTHER_STATE_WAIT);
	objects.push_back(panther1);

	Panther *panther2 = new Panther();
	panther2->AddAnimation(605);
	panther2->AddAnimation(606);
	panther2->AddAnimation(607);
	panther2->AddAnimation(608);
	panther2->SetPosition(1950, 260);
	panther2->SetState(PANTHER_STATE_WAIT);
	objects.push_back(panther2);

	for (int i = 0; i < 4; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1247 + i * 32, 405 - i * 32);
		objects.push_back(stair);
	}
	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1439 + i * 32.0, 278 - i * 32);
		objects.push_back(stair);
	}
	
	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(802);
		stair->SetPosition(1824 + i * 32.0, 246 + i * 32);
		objects.push_back(stair);

	}
	for (int i = 0; i < 6; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(2590 + i * 32.0, 410 - i * 32);
		objects.push_back(stair);
	}
	
	CheckStair *checkstair;
	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1255, 407); // 379
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	objects.push_back(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1382, 215);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	objects.push_back(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1448, 280);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	objects.push_back(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1512, 151);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	objects.push_back(checkstair);

	// Top right
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1790, 151);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	objects.push_back(checkstair);

	// Bottom left
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1850, 280);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	objects.push_back(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(2595, 407);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	objects.push_back(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(2788, 153);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	objects.push_back(checkstair);
	//1250 335 1265 320 1280 305*/ 3 10 6 8
	for (int i = 0; i < 2; i++)
	{
		Zombie *zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->AddAnimation(604);
		zombie->SetPosition(500 + i * 64, 376);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		objects.push_back(zombie);
	}
}
void LoadResourceLv2_1()
{
	
	for (int i = 0; i < 9; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + i*32 +32*8 , 246);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 +32*9 + i * 32.0f, 440);
		objects.push_back(ground);
	}
	for (int i = 0; i < 12; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 13 + i * 32.0f + 3, 440);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 440);
		objects.push_back(ground);
	}
	for (int i = 0; i < 8; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 33+3 + i * 32.0f, 440);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)//
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 17 + i * 32.0f, 246+64);
		objects.push_back(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 246+32);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 31 + i * 32.0f, 246+64);
		objects.push_back(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 35 +3 + i * 32.0f, 246);
		objects.push_back(ground);
	}
	Candle *candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3245, 193);
	objects.push_back(candle);

	Candle *candle1 = new Candle();
	candle1->AddAnimation(800);
	candle1->SetPosition(3389, 159);
	objects.push_back(candle1);
	
	Candle *candle2 = new Candle();
	candle2->AddAnimation(800);
	candle2->SetPosition(3137, 386);
	objects.push_back(candle2);

	Candle *candle3 = new Candle();
	candle3->AddAnimation(800);
	candle3->SetPosition(3519, 319);
	objects.push_back(candle3);

	Candle *candle4 = new Candle();
	candle4->AddAnimation(800);
	candle4->SetPosition(3684, 155);
	objects.push_back(candle4);

	Candle *candle5 = new Candle();
	candle5->AddAnimation(800);
	candle5->SetPosition(3776, 385);
	objects.push_back(candle5);

	Candle *candle6 = new Candle();
	candle6->AddAnimation(800);
	candle6->SetPosition(3905, 150);
	objects.push_back(candle6);

	Candle *candle7 = new Candle();
	candle7->AddAnimation(800);
	candle7->SetPosition(4035, 192);
	objects.push_back(candle7);

}
void LoadResourceLv2_2()
{}
void LoadResourceboss()
{
	for (int i = 0; i < 8; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 40  + i * 32.0f, 246);
		objects.push_back(ground);
	}
	for (int i = 0; i < 48; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 41 + i * 32.0f, 438  );
		objects.push_back(ground);
	}
	for (int i = 0; i < 11; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 49 + i * 32.0f, 310);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 86 + i * 32.0f, 310);
		objects.push_back(ground);
	}
	for (int i =0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 83 + i * 32.0f, 374);
		objects.push_back(ground);
	}
	
	Candle *candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4285, 194);
	objects.push_back(candle);

	Candle *candle1 = new Candle();
	candle1->AddAnimation(800);
	candle1->SetPosition(4418, 255);
	objects.push_back(candle1);

	Candle *candle2 = new Candle();
	candle2->AddAnimation(800);
	candle2->SetPosition(4362, 380);
	objects.push_back(candle2);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(4866 + i * 127, 380);
		objects.push_back(candle);
	}
	
	for (int i = 0; i < 2; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(5438 + i * 127, 310);
		objects.push_back(candle);
	}
	
	BossBat *bossbat = new BossBat();
	bossbat->AddAnimation(609);
	bossbat->AddAnimation(610);
	bossbat->SetPosition(5325, 125);
	bossbat->SetState(BOSSBAT_STATE_WAIT);
	objects.push_back(bossbat);
}

void Update(DWORD dt)
{
	float x, y;
	simon->GetPosition(x, y);
	if (!simon->isLevelUp)
		gameTime -= dt;
#pragma region Resource
	if (lv1 == true)
	{
		Item *bonus = NULL;
		// Thưởng bonus vòng 1
		if (simon->isBonus)
		{
			bonus = new Item();
			bonus->SetPosition(1367, 407);
			bonus->AddAnimation(ITEM_MONEY);
			bonus->SetType(ITEM_MONEY);
			simon->isBonus = false;
		}

		if (bonus != NULL)
		{
			if (bonus->y > 370)
			{
				bonus->y -= SIMON_CLIMBING_SPEED_Y * dt;
				bonus->Render();
			}
			else
			{
				objects.push_back(bonus);
			}
		}

		// Lên cấp
		/*if (simon->isLevelUp) {
			simon->SetState(SIMON_STATE_WALK);
			simon->SetSpeed(SIMON_LEVELUP_SPEED, 0);
			DWORD timer = GetTickCount();
			if (timer - simon->onCheckPointTime > LEVELUP_TIME)
			{
				for (int i = objects.size() - 1; i > 0; i--)
					objects.pop_back();

				lv2 = true;
				lv1 = false;
				simon->isLevelUp = false;
				simon->SetState(SIMON_STATE_IDLE);
			}
		}*/
	}
	if (lv2 == true)
	{
		if (countLoadResourceLv2 == false)
		{
			game->mCamera->setX(0);
			LoadResourceLv2();
			countLoadResourceLv2 = true;
			simon->SetPosition(50, 155);
			timer = GetTickCount();
		}
		else if(countLoadResourceLv2 == true && x < MAX_WIDTH_LV2 - 2*SIMON_STAND_BBOX_WIDTH)
		{
			if (GetTickCount() - timer > 5000)
			{
				Zombie *zombie = new Zombie();
				zombie->AddAnimation(602);
				zombie->AddAnimation(604);
				zombie->SetPosition(1200, 376);
				zombie->SetState(ZOMBIE_STATE_WALKING);
				objects.push_back(zombie);
				timer = timer + 5000;
			}
		}
		else  //check point
		{
			lv2_1 = true;
			lv2 = false;
		}
	}
	if (lv2_1 == true)
	{
		
		if (countLoadResourceLv2_1 == false)
		{
			for (int i = objects.size()-1; i > 0; i--)
				objects.pop_back();			
			LoadResourceLv2_1();
			countLoadResourceLv2_1 = true;
		}
		 if (x > MAX_WIDTH_LV2_1 - 2*SIMON_STAND_BBOX_WIDTH)
		{
			lv2_1 = false;			
			boss = true;

		}
		
	}
	if (lv2_2 == true)
	{
		if (countLoadResourceLv2_2 == false)
		{
			for (int i = objects.size() - 1; i > 200; i--)
				objects.pop_back();
			countLoadResourceLv2_2 = true;
			simon->SetPosition(50, 150);
		}
	}
	if (boss == true)
	{
		if (countLoadResourceboss == false)
		{
			for (int i = objects.size() - 1; i > 0; i--)
				objects.pop_back();
			LoadResourceboss();
			countLoadResourceboss = true;
			timer2 = GetTickCount();
		}
		else if(countLoadResourceboss == true)
		{
			if (GetTickCount() - timer2 > 5000)
			{
				Zombie *zombie = new Zombie();
				zombie->AddAnimation(602);
				zombie->AddAnimation(604);
				zombie->SetPosition(4900, 376);
				zombie->SetState(ZOMBIE_STATE_WALKING);
				objects.push_back(zombie);
				timer2 = timer2 + 5000;
			}
		}
		
	}

#pragma endregion

#pragma region Collision
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	if(boss == false)
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Update( dt, &coObjects);
		}
	else if(boss == true)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<BossBat *>(objects.at(i)))
			{
				BossBat *bossbat = dynamic_cast<BossBat *>(objects.at(i));
				bossbat->Update(x, y, dt, &coObjects);
			}
			else
				objects[i]->Update( dt, &coObjects);
		}
		

	}

	
#pragma endregion	

#pragma region Remove Object
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Zombie *>(objects.at(i)))
		{
			Zombie *zombie = dynamic_cast<Zombie *>(objects.at(i));

			if (zombie->GetState() == ZOMBIE_STATE_DIE)
			{
				objects.erase(objects.begin() + i);
				delete zombie;
			}
		}
		//else if (dynamic_cast<BigFire *>(objects.at(i)))
		//{
		//	BigFire *bigFire = dynamic_cast<BigFire *>(objects.at(i));
		//	if (bigFire->isHitted)
		//	{
		//		float bigfire_x, bigfire_y, bigfire_right, bigfire_bottom;
		//		bigFire->GetBoundingBox(bigfire_x, bigfire_y, bigfire_right, bigfire_bottom);
		//		
		//		item = new Item();
		//		item->SetPosition(bigfire_x, bigfire_y);
		//		item->SetSpeed(0, -0.1);
		//		objects.push_back(item);

		//		// Whip item
		//		if (simon->whip->level < 2)
		//		{
		//			item->AddAnimation(ITEM_WHIPITEM);
		//			item->SetType(ITEM_WHIPITEM);
		//		}
		//		else
		//		{
		//			/**
		//			 * Random ra item: (do hiện tại chỉ có 3 món này)
		//			 * 95% heart
		//			 * 4% money
		//			 * 1% knife
		//			 */

		//			srand(time(NULL));
		//			int random_portion = rand() % 100;

		//			// Heart
		//			if (random_portion < 95)
		//			{
		//				item->AddAnimation(ITEM_HEART);
		//				item->SetType(ITEM_HEART);
		//			}
		//			// Money
		//			else if (random_portion >= 95 && random_portion < 99)
		//			{
		//				item->AddAnimation(ITEM_MONEY);
		//				item->SetType(ITEM_MONEY);
		//			}
		//			// Knife
		//			else
		//			{
		//				item->AddAnimation(ITEM_KNIFE);
		//				item->SetType(ITEM_KNIFE);
		//			}
		//		}

		//		// Thêm hiệu ứng tóe lửa
		//		whipEffect = new Effect(GetTickCount());
		//		whipEffect->AddAnimation(806);
		//		whipEffect->SetPosition(bigfire_x, bigfire_y + (bigfire_bottom - bigfire_y) / 4);
		//		objects.push_back(whipEffect);

		//		whipEffect = new Effect(GetTickCount());
		//		whipEffect->AddAnimation(807);
		//		whipEffect->SetPosition(bigfire_x, bigfire_y + (bigfire_bottom - bigfire_y) / 4);
		//		objects.push_back(whipEffect);

		//		objects.erase(objects.begin() + i);
		//		delete bigFire;
		//	}
		//}
		else if (dynamic_cast<Lantern *>(objects.at(i)))
		{
			Lantern *lantern = dynamic_cast<Lantern *>(objects.at(i));
			if (lantern->isShooted)
			{
				float lantern_x, lantern_y, lantern_right, lantern_bottom;
				lantern->GetBoundingBox(lantern_x, lantern_y, lantern_right, lantern_bottom);

				item = new Item();
				item->SetPosition(lantern_x, lantern_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);

				// Whip item
				if (simon->whip->level < 2)
				{
					item->AddAnimation(ITEM_WHIPITEM);
					item->SetType(ITEM_WHIPITEM);
				}
				else
				{
					/**
					 * Random ra item: (do hiện tại chỉ có 3 món này)
					 * 95% heart
					 * 4% money
					 * 1% knife
					 */

					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 95)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Money
					else if (random_portion >= 95 && random_portion < 99)
					{
						item->AddAnimation(ITEM_MONEY);
						item->SetType(ITEM_MONEY);
					}
					// Knife
					else
					{
						item->AddAnimation(ITEM_KNIFE);
						item->SetType(ITEM_KNIFE);
					}
				}

				// Thêm hiệu ứng tóe lửa
				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(806);
				whipEffect->SetPosition(lantern_x, lantern_y + (lantern_bottom - lantern_y) / 4);
				objects.push_back(whipEffect);

				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(807);
				whipEffect->SetPosition(lantern_x, lantern_y + (lantern_bottom - lantern_y) / 4);
				objects.push_back(whipEffect);

				objects.erase(objects.begin() + i);
				delete lantern;
			}
		}
		else if (dynamic_cast<Item *>(objects.at(i)))
		{
			Item *item = dynamic_cast<Item *>(objects.at(i));

			if (item->GetEaten())
			{
				objects.erase(objects.begin() + i);
				delete item;
			}
		}
		else if (dynamic_cast<Effect *>(objects.at(i)))
		{
			Effect *effect = dynamic_cast<Effect *>(objects.at(i));

			if (effect->GetExposed())
			{
				objects.erase(objects.begin() + i);
				delete effect;
			}
		}
	}
#pragma endregion	

#pragma region Camera
	if (lv1 == true)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{

			game->mCamera->setX(x - SCREEN_WIDTH / 2);
			game->mCamera->setY(0);
		}
		else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->mCamera->setX(MAX_WIDTH_LV1 - SCREEN_WIDTH);
			game->mCamera->setY(0);
		}
		else
		{
			game->mCamera->setX(0);
			game->mCamera->setY(0);
		}
	}
	else if( lv2 == true)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
		{
			game->mCamera->setX(x - SCREEN_WIDTH / 2);
			game->mCamera->setY(0);
		}
		else if (x > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) {
			game->mCamera->setX(MAX_WIDTH_LV2 - SCREEN_WIDTH);
			game->mCamera->setY(0);
		}
		else if(x < SCREEN_WIDTH / 2)
		{
			game->mCamera->setX(0);
			game->mCamera->setY(0);
		}

	}
	//else if (lv2_1 == true)
	//{
	//	
	//	// chuyen scene
	//	if (game->x_cam < MAX_WIDTH_LV2 - SCREEN_WIDTH/2)
	//	{
	//		game->x_cam += SIMON_WALKING_SPEED * dt;
	//		game->y_cam = 0;
	//		
	//	}
	//	
	//	else {
	//		
	//		if (x < 3200)
	//		{
	//			if(check1 == false)
	//			simon->SetState(SIMON_STATE_WALK);
	//			
	//		}
	//		if (x >3200 && x<3210 ) {
	//			check1 = true;
	//			check = true;
	//			simon->SetState(SIMON_STATE_IDLE);
	//						
	//		}
	//		
	//	}
	//	if ( game->x_cam < MAX_WIDTH_LV2   && check == true)
	//	{
	//		game->x_cam += SIMON_WALKING_SPEED * dt;
	//		game->y_cam = 0;
	//		
	//	}
	//	else if (game->x_cam > MAX_WIDTH_LV2)
	//	{
	//		checkScene = true;
	//	}
	//	
	//	
	//	
	//	//
	//	//trả camera về simon
	//	if (checkScene == true)
	//	{
	//		if ( x > MAX_WIDTH_LV2 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
	//		{
	//			game->x_cam = x - SCREEN_WIDTH / 2;
	//			game->y_cam = 0;
	//		}
	//		else if (x > MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2) {
	//			game->x_cam = MAX_WIDTH_LV2_1 - SCREEN_WIDTH;
	//			game->y_cam = 0;
	//		}
	//		else if (x < MAX_WIDTH_LV2 + SCREEN_WIDTH / 2)
	//		{
	//			game->x_cam = MAX_WIDTH_LV2;
	//			game->y_cam = 0;
	//		}
	//	}
	//	
	//}
	else if (lv2_2 == true)
	{
		game->mCamera->setX(0);
		game->mCamera->setY(0);
	}
	else if (boss == true)
	{
		if (game->mCamera->getX() < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
		{
			

			game->mCamera->setX(game->mCamera->getX() + SIMON_WALKING_SPEED * dt);
			game->mCamera->setY(0);
			

		}
		else {

			if (x < MAX_WIDTH_LV2_1 + 100)
			{
				if(check3 == false)
				simon->SetState(SIMON_STATE_WALK);
			}
			else if (x > MAX_WIDTH_LV2_1 + 100 && x < MAX_WIDTH_LV2_1 + 105)
			{
				simon->SetState(SIMON_STATE_IDLE);
				checkScene1 = true;
				check3 = true;
			}
		}
		
		if (checkScene1 == true)
		{
			if (game->mCamera->getX() < MAX_WIDTH_LV2_1)
				game->mCamera->setX(game->mCamera->getX() + SIMON_WALKING_SPEED * dt);
			else
			{
				if (x > MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
				{
					game->mCamera->setX(x - SCREEN_WIDTH / 2);
					game->mCamera->setY(0);

				}
				else if (x > MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2) {
					game->mCamera->setX(MAX_WIDTH_LV2_2 - SCREEN_WIDTH);
					game->mCamera->setY(0);

				}
				else if (x < MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2)
				{
					game->mCamera->setX(MAX_WIDTH_LV2_1);
					game->mCamera->setY(0);

				}
			}
		}
	}
#pragma endregion

#pragma region UI
	/*if (lv1 == true)
		ui->Update(gameTime/1000, 1,simon);
	else
		ui->Update(gameTime/1000, 2,simon);*/
#pragma endregion

}


void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 tex = textures->Get(ID_TEX_LV1);
		LPDIRECT3DTEXTURE9 tex2 = textures->Get(ID_TEX_LV1_2);		
		float x, y;
		simon->GetPosition(x, y);

		LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);
		LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET_2);
		LPDIRECT3DTEXTURE9 tileset2 = textures->Get(ID_TEX_TILESET_3);
		
		if (lv1 == true)
		{
			
			map = new	Map (/*48, 10,*/ tileset, 16, 16); 
			map->LoadMatrixMap("Resources\\Mapstate.txt");
			//map->Draw(game->x_cam, game->y_cam);
		}
		else if( lv2 == true || lv2_1 == true) {									
			map = new	Map (/*176, 11,*/ tileset1, 32, 32); 
			map->LoadMatrixMap("Resources\\Mapstate2.txt");
			//map->Draw(game->x_cam , game->y_cam);

		}
		else if (lv2_2 == true)
		{
			map = new	Map(/*32, 11,*/ tileset2, 32, 32);
			map->LoadMatrixMap("Resources\\Mapstate2_1.txt");
			
		}
		map->Draw(game->mCamera->getX(), game->mCamera->getY());
		for (int i = 1; i < objects.size(); i++)
			objects[i]->Render();

		objects[0]->Render();
		//ui->Render(game->x_cam,game->y_cam,simon);
		/*RECT newRect;
		SetRect(&newRect, 0, 0, 30, 30);
		D3DXVECTOR3 p(0, 0, 0);
		spriteHandler->Draw(textures->Get(ID_TEX_AXE), &newRect, NULL, &p, D3DCOLOR_ARGB(255, 255, 255, 255));*/
		
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	
	

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}