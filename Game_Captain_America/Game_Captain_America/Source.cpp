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
#include "Shield.h"
#include "Captain.h"
#include "Ground.h"
#include "define.h"
#include "Zombie.h"
#include "Item.h"
#include "Lantern.h"
#include "Map.h"
#include "CheckPoint.h"
#include "UI.h"
#include "Water.h"
#include "GridObjects.h"
#include "Soldier.h"
CGame *game;
Captain * captain;
Item *item;
Effect *whipEffect;
Map *map;
Shield *knife;
//UI * ui;
CSprite *sprite;
//vector<LPGAMEOBJECT> objects;
//vector<int> willDeleteObjects;

ListGrids *listGrids;
vector<GridObjects*> currentGrids;

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
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Nhay
	if (KeyCode == DIK_Z)
	{
		if (captain->isJump == false && captain->isSit == false && captain->isAttack == false && captain->isOnStair == false)
			captain->SetAction(SIMON_ACTION_JUMP);
	}

	if (KeyCode == DIK_C)
	{
		if (captain->isDoubleJump == false && captain->isSit == false && captain->isAttack == false)
			captain->SetState(SIMON_STATE_DOUBLE_JUMP);
	}
	if (KeyCode == DIK_X)
	{
		captain->SetCurrentWeapon(809);
		if (captain->isAttack == false) {

			int nx = captain->nx;
			captain->SetAction(SIMON_ACTION_ATTACK);
			switch (captain->currentWeapon)
			{
			case ITEM_KNIFE:
				knife = new Shield(captain, 2 * SCREEN_WIDTH / 3);
				if (nx > 0)
				{
					knife->SetSpeed(KNIFE_SPEED, 0);
					knife->AddAnimation(WEAPON_ANI_KNIFE);
				}
				else if (nx < 0)
				{
					knife->SetSpeed(-KNIFE_SPEED, 0);
					knife->AddAnimation(WEAPON_ANI_KNIFE);
				}
				knife->SetType(ITEM_KNIFE);
				knife->SetPosition(captain->x, captain->y);
				knife->appearTime = GetTickCount();
				knife->firstCast = GetTickCount();
				//objects.push_back(knife);
				listGrids->AddObject(knife);
				break;
			}
		}
	}

}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Chet
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (KeyCode == DIK_UP)
	{
		if (captain->isHoldShield)
		{
			captain->SetState(SIMON_STATE_IDLE);
			captain->isHoldShield = false;
		}
	}

	if (KeyCode == DIK_Q) {
		captain->SetState(SIMON_STATE_IDLE);
		captain->isDashing = false;

	}
	if (KeyCode == DIK_C) {
		captain->SetState(SIMON_STATE_IDLE);
		captain->isDoubleJump = false;
		captain->isJump = false;
	}
	// Ngoi
	if (KeyCode == DIK_DOWN)
	{

		 if (captain->isSit)
		{
			if (!captain->isAttack)
			{
				captain->isSit = false;
			}
			else
			{
				captain->isExitSit = true;
			}
		}
	}

	// Di bo
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
	{
		captain->isMoving = false;
		captain->vx = 0;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (game->IsKeyDown(DIK_UP))
	{
		//if (!captain->isOnCheckStairDown && !captain->isOnStair && !captain->isAttack && !captain->isJump)
		captain->SetState(SIMON_STATE_HOLD_SHIELD);
	}

	// Ngoi
	if (game->IsKeyDown(DIK_Q)) {
		captain->SetState(SIMON_STATE_DASHING);
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		 if (!captain->isAttack && !captain->isJump)
			captain->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!captain->isSit && !captain->isAttack)
			captain->SetState(SIMON_STATE_WALK);
		if (!captain->isJump &&  !captain->isAttack)
			captain->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!captain->isSit && !captain->isAttack)
			captain->SetState(SIMON_STATE_WALK);
		if (!captain->isJump && !captain->isAttack)
			captain->nx = -1.0f;
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
	textures->Add(ID_TEX_KNIFE, L"Resources\\captian.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_MARIO, L"Resources\\captian.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_MARIO_2, L"Resources\\american.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_LV1, L"Resources\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Resources\\maplv2.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BRICK, L"Resources\\2.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_BRICK2, L"Resources\\BRICK1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ZOMBIE, L"Resources\\enemy.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_ZOMBIE_RIGHT, L"Resources\\enemy.png", D3DCOLOR_XRGB(173, 214, 214));
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
	//textures->Add(ID_TEX_KNIFE, L"Resources\\UI\\KNIFE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_NOHP, L"Resources\\UI\\NoHP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_STOP_WATCH, L"Resources\\UI\\STOP_WATCH.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_RECT, L"Resources\\rect.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LANTERN, L"Resources\\lantern.png", D3DCOLOR_XRGB(255,0,255));
	textures->Add(ID_TEX_WATER, L"Resources\\Water.png", D3DCOLOR_XRGB(255, 0, 255));


	textures->Add(ID_TEX_EFFECT1, L"Resources\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT2, L"Resources\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_VIP, L"Resources\\WHIP_VIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONEY, L"Resources\\money_bag_red.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIT, L"Resources\\sitstate.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_ITEM1, L"Resources\\item1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM2, L"Resources\\item2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM3, L"Resources\\item3.png", D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texShield = textures->Get(ID_TEX_KNIFE);
	LPDIRECT3DTEXTURE9 texCaptain = textures->Get(ID_TEX_MARIO);
	LPDIRECT3DTEXTURE9 texCaptainSit = textures->Get(ID_TEX_SIT);
	#pragma region Addsprite


	sprites->Add(10001, 232, 3, 258, 45, texCaptain);		// đứng im phải

	sprites->Add(10002, 233, 52, 260, 94, texCaptain);		// đi phải
	sprites->Add(10003, 260, 51, 286, 94, texCaptain);
	sprites->Add(11002, 233, 52, 260, 94, texCaptain);
	sprites->Add(11003, 260, 51, 286, 94, texCaptain);

	sprites->Add(10004, 231, 138, 273, 178, texCaptain);		// đánh phải			
	sprites->Add(10005, 273, 138, 315, 178, texCaptain);
	//sprites->Add(10006, 0, 0, 64, 64, texCaptain);

	sprites->Add(10028, 259, 264, 297, 311, texCaptainSit);		// ngồi đánh phải			
	sprites->Add(10029, 299, 263, 328, 310, texCaptainSit);


	sprites->Add(10018, 285, 146, 311, 191, texCaptainSit); // ngồi phải

	sprites->Add(10020, 233, 96, 255, 135, texCaptain); // nhảy phải

	sprites->Add(10054, 436, 67, 471, 130, texCaptain);//bị đánh từ bên trái

	LPDIRECT3DTEXTURE9 texCaptain2 = textures->Get(ID_TEX_MARIO);
	sprites->Add(10011, 203, 3, 227, 48, texCaptain2);		// đứng im trái

	sprites->Add(10012, 203, 52, 230, 94, texCaptain2);		// đi trái
	sprites->Add(10013, 175, 51, 199, 94, texCaptain2);
	sprites->Add(11012, 203, 52, 230, 94, texCaptain2);
	sprites->Add(11013, 175, 51, 199, 94, texCaptain2);

	sprites->Add(10014, 189, 138, 229, 177, texCaptain2);		// đánh trái				
	sprites->Add(10015, 146, 138, 187, 177, texCaptain2);
	//sprites->Add(10016, 421, 0, 464, 64, texCaptain2);

	sprites->Add(10031, 216, 265, 257, 310, texCaptainSit);		// ngồi đánh trái				
	sprites->Add(10032, 185, 265, 217, 310, texCaptainSit);

	sprites->Add(10019, 253, 147, 281, 191, texCaptainSit); // ngồi trái

	sprites->Add(10021, 203, 97, 229, 136, texCaptain2);//nhảy trái

	sprites->Add(10055, 12, 70, 41, 130, texCaptain2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texCaptain);		// chết 

	sprites->Add(10110, 106, 144, 149, 189, texCaptainSit);//nhay danh phai

	sprites->Add(10111, 59, 144, 102, 189, texCaptainSit); // nhay danh trai

	sprites->Add(10120, 282, 96, 302, 136, texCaptain); // double nhay 
	sprites->Add(10121, 305, 96, 327, 136, texCaptain);
	sprites->Add(10122, 327, 96, 349, 136, texCaptain);

	sprites->Add(10130, 237, 79, 276, 123, texCaptainSit);// dashing left
	sprites->Add(10131, 310, 79, 348, 123, texCaptainSit);// dashing right

	sprites->Add(10140, 233, 224, 258, 268, texCaptain); // knifeing right
	sprites->Add(10141, 202, 224, 227, 268, texCaptain);	//knifeing left
	sprites->Add(1200, 315, 153, 333, 192, texShield);	//khien

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
	sprites->Add(30001, 135, 12, 161, 56, texEnemy); // đi trái 
	sprites->Add(30002, 104, 12, 127, 56, texEnemy);
	sprites->Add(30003, 72, 12, 97, 56, texEnemy);

	sprites->Add(30007, 623, 20, 650, 56, texEnemy); //chet right



	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add(30004, 719, 12, 743, 56, texEnemy0); // đi Phải 
	sprites->Add(30005, 753, 12, 775, 56, texEnemy0);
	sprites->Add(30006, 783, 12, 808, 56, texEnemy0);

	sprites->Add(30008, 231, 21, 257, 56, texEnemy0);//chet left

		//Soldier
	sprites->Add(33000, 720, 66, 744, 112, texEnemy); // di phai
	sprites->Add(33001, 752, 66, 775, 112, texEnemy);
	sprites->Add(33002, 783, 66, 809, 112, texEnemy);

	sprites->Add(33003,	137, 66, 161, 112, texEnemy); //di trai
	sprites->Add(33004, 104, 66, 126, 112, texEnemy);
	sprites->Add(33005, 71, 66, 96, 112, texEnemy);

	sprites->Add(34000, 653, 66, 681, 112, texEnemy); //chet phai

	sprites->Add(34001, 200,66, 224,112, texEnemy); //chet trai

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

	LPDIRECT3DTEXTURE9 texMisc14 = textures->Get(ID_TEX_WATER);
	sprites->Add(40026, 0, 0, 16, 16, texMisc14);
	sprites->Add(40027, 16, 0, 32, 32, texMisc14);

	LPDIRECT3DTEXTURE9 texMisc15 = textures->Get(ID_TEX_ITEM1);
	sprites->Add(40028, 0, 0, 18, 20, texMisc15);

	LPDIRECT3DTEXTURE9 texMisc16 = textures->Get(ID_TEX_ITEM2);
	sprites->Add(40029, 0, 0,18,21, texMisc16);

	LPDIRECT3DTEXTURE9 texMisc17 = textures->Get(ID_TEX_ITEM3);
	sprites->Add(40030, 0, 0, 16, 24, texMisc17);
	#pragma endregion

	LPANIMATION ani;

	#pragma region CaptainAnimation

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
		

	ani = new CAnimation(100); //knifeing right
	ani->Add(10140);
	animations->Add(1605, ani);

	ani = new CAnimation(100); //knifeing left
	ani->Add(10141);
	animations->Add(1606, ani);

	ani = new CAnimation(100);
	ani->Add(1200);
	animations->Add(2222, ani); //nem khien

	#pragma endregion

	#pragma region ObjectAnimation

	ani = new CAnimation(100);	//đất1
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100); //zombie đi trái
	ani->Add(30001);
	ani->Add(30002);
	ani->Add(30003);
	animations->Add(602, ani);

	ani = new CAnimation(100); //zombie đi phải
	ani->Add(30004);
	ani->Add(30005);
	ani->Add(30006);
	animations->Add(604, ani);

	ani = new CAnimation(100);//zombie die right
	ani->Add(30007);
	animations->Add(850, ani);

	ani = new CAnimation(100);//zombie die left
	ani->Add(30008);
	animations->Add(851, ani);


	ani = new CAnimation(100); //soldier đi phải
	ani->Add(33000);
	ani->Add(33001);
	ani->Add(33002);
	animations->Add(900, ani);

	ani = new CAnimation(100); //soldier đi trái
	ani->Add(33003);
	ani->Add(33004);
	ani->Add(33005);
	animations->Add(901, ani);

	ani = new CAnimation(100); //soldier chet phai
	ani->Add(34000);
	animations->Add(902, ani);
	
	ani = new CAnimation(100); //soldier chet trai
	ani->Add(34001);
	animations->Add(903, ani);

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

	ani = new CAnimation(100); //water
	ani->Add(40026);
	ani->Add(40027);
	animations->Add(812, ani);


	ani = new CAnimation(0); //item 1
	ani->Add(40028);
	animations->Add(813, ani);

	ani = new CAnimation(0); //item 2
	ani->Add(40029);
	animations->Add(814, ani);

	ani = new CAnimation(0); //item 3 
	ani->Add(40030);
	animations->Add(815, ani);
	#pragma endregion

	#pragma region captain
	captain = new Captain();
	captain->AddAnimation(400);	// đứng phải	
	captain->AddAnimation(401);	//đứng trái	
	captain->AddAnimation(500);	// đi phải
	captain->AddAnimation(501);	//đi trái
	captain->AddAnimation(402);	//đánh phải 
	captain->AddAnimation(403);	//đánh trái
	captain->AddAnimation(404);	//ngồi phải
	captain->AddAnimation(405);	//ngồi trái
	captain->AddAnimation(406);	//nhảy phải 
	captain->AddAnimation(407);	//nhảy trái
	captain->AddAnimation(410);	//ngồi đánh phải 
	captain->AddAnimation(411);	//ngồi đánh trái
	captain->AddAnimation(412);	//trên thang phải đánh phải
	captain->AddAnimation(413);	//trên thang phải đánh trái
	captain->AddAnimation(414);	//trên thang trái đánh phải
	captain->AddAnimation(415);	//trên thang trái đánh trái
	captain->AddAnimation(416);	//lên thang phải
	captain->AddAnimation(417);	//xuống thang phải
	captain->AddAnimation(418);	//lên thang trái
	captain->AddAnimation(419);	//xuống thang trái
	captain->AddAnimation(420);	//đứng im bên phải thang phải
	captain->AddAnimation(421);	//đứng im bên phải thang trái
	captain->AddAnimation(422);	//đứng im bên trái thang phải
	captain->AddAnimation(423);	//đứng im bên trái thang trái
	captain->AddAnimation(424);	//bị đánh từ bên trái
	captain->AddAnimation(425);	//bị đánh từ bên phải
	captain->AddAnimation(599);	//chết
	captain->AddAnimation(1600); // nhay danh phai
	captain->AddAnimation(1601); // nhay danh trai
	captain->AddAnimation(1602); // double nhay
	captain->AddAnimation(1603); // dashing right
	captain->AddAnimation(1604); // dashing left
	captain->AddAnimation(1605);	//knifeing right
	captain->AddAnimation(1606);	//knifeing left

	
	captain->SetPosition(15, 160);
	//objects.push_back(captain);

	// khởi tạo listgrids
	listGrids->InitList(MAX_WIDTH_LV1);
	//listGrids->AddObject(captain);

	#pragma endregion

	#pragma region Ground
	// nền di chuyển
	for (int i = 0; i < 128; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 16.0f, 245);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// nước
	for (int i = 0; i < 128; i++)
	{
		Water *water = new Water();
		water->AddAnimation(812);
		water->SetPosition(0 + i * 16.0f, 258);
		//objects.push_back(water);
		listGrids->AddObject(water);
	}

	for (int i = 0; i < 128; i++)
	{
		Water *water = new Water();
		water->AddAnimation(812);
		water->SetPosition(0 + i * 16.0f, 274);
		//objects.push_back(water);
		listGrids->AddObject(water);
	}

	// vật cản trên 1
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(48 + i * 16.0f, 120);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 2
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(576 + i * 16.0f, 164);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 3
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(624 + i * 16.0f, 88);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 4
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(672 + i * 16.0f, 164);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 5
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(832 + i * 16.0f, 164);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 6
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(880 + i * 16.0f, 88);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 7
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(926 + i * 16.0f, 164);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản trên 8
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(1332 + i * 16.0f, 120);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}

	// vật cản 9
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(1792 + i * 16.0f, 229);
		//objects.push_back(ground);
		listGrids->AddObject(ground);
	}
	#pragma endregion

#pragma region Zombie
		Zombie *zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->AddAnimation(604);
		zombie->SetPosition(300, 200);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		//objects.push_back(zombie);
		listGrids->AddObject(zombie);


	Zombie *zombie1 = new Zombie();
	zombie1->AddAnimation(602);
	zombie1->AddAnimation(604);
	zombie1->SetPosition(400, 200);
	zombie1->SetState(ZOMBIE_STATE_WALKING);
	//objects.push_back(zombie1);
	listGrids->AddObject(zombie1);

#pragma endregion
#pragma region Soldier
	Soldier *soldier = new Soldier();
	soldier->AddAnimation(901);
	soldier->AddAnimation(900);
	soldier->SetPosition(800, 200);
	soldier->SetState(SOLDIER_STATE_WALKING);
	//objects.push_back(zombie1);
	listGrids->AddObject(soldier);

	Soldier *soldier1 = new Soldier();
	soldier1->AddAnimation(901);
	soldier1->AddAnimation(900);
	soldier1->SetPosition(1000, 200);
	soldier1->SetState(SOLDIER_STATE_WALKING);
	//objects.push_back(zombie1);
	listGrids->AddObject(soldier1);
#pragma endregion
#pragma region Lantern
	Lantern *lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(64, 80);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(128, 180);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	//lantern3->SetPosition(318, 188);
	lantern->SetPosition(318, 184);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(414, 96);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(574, 128);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(832, 128);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1088, 180);
	//objects.push_back(lantern7);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1184, 96);
	//objects.push_back(lantern8);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1344,80);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1408, 172);
	//objects.push_back(lantern10);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1600, 180);
	//objects.push_back(lantern11);
	listGrids->AddObject(lantern);

	lantern = new Lantern();
	lantern->AddAnimation(811);
	lantern->SetPosition(1696, 96);
	//objects.push_back(lantern);
	listGrids->AddObject(lantern);
#pragma endregion

#pragma region CheckPoint
	CheckPoint *checkPoint;
	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_LEVELUP);
	checkPoint->SetPosition(2048, 374);
	//objects.push_back(checkPoint);
	listGrids->AddObject(checkPoint);

	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_BONUS);
	checkPoint->SetPosition(2020, 374);
	//objects.push_back(checkPoint);
	listGrids->AddObject(checkPoint);
#pragma endregion

	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	
	/*ui = new UI();
	ui->Initialize(d3ddv,captain);*/
}

//void LoadResourceLv2() {
//	for (int i = 0; i < 95; i++)
//	{
//		Ground *ground = new Ground();
//
//		ground->SetPosition(0 + i * 32.0f, 440);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		Ground *ground = new Ground();
//
//		ground->SetPosition(1377 + i * 32.0f, 310);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 10; i++)
//	{
//		Ground *ground = new Ground();
//
//		ground->SetPosition(1503 + i * 32.0f, 244);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 6; i++)
//	{
//		Ground *ground = new Ground();
//
//		ground->SetPosition(1854 + i * 32.0f, 310);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 10; i++)
//	{
//		Ground *ground = new Ground();
//
//		ground->SetPosition(2782 + i * 32.0f, 246);
//		objects.push_back(ground);
//	}
//	
//	//1250 335 1265 320 1280 305*/ 3 10 6 8
//	for (int i = 0; i < 2; i++)
//	{
//		Zombie *zombie = new Zombie();
//		zombie->AddAnimation(602);
//		zombie->AddAnimation(604);
//		zombie->SetPosition(500 + i * 64, 376);
//		zombie->SetState(ZOMBIE_STATE_WALKING);
//		objects.push_back(zombie);
//	}
//}
//void LoadResourceLv2_1()
//{
//	
//	for (int i = 0; i < 9; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + i*32 +32*8 , 246);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 +32*9 + i * 32.0f, 440);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 12; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 13 + i * 32.0f + 3, 440);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 440);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 8; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 33+3 + i * 32.0f, 440);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)//
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 17 + i * 32.0f, 246+64);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 2; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 246+32);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 31 + i * 32.0f, 246+64);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 6; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 35 +3 + i * 32.0f, 246);
//		objects.push_back(ground);
//	}
//}
//void LoadResourceLv2_2()
//{}
//void LoadResourceboss()
//{
//	for (int i = 0; i < 8; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 40  + i * 32.0f, 246);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 48; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 41 + i * 32.0f, 438  );
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 11; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 49 + i * 32.0f, 310);
//		objects.push_back(ground);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 86 + i * 32.0f, 310);
//		objects.push_back(ground);
//	}
//	for (int i =0; i < 2; i++)
//	{
//		Ground *ground = new Ground();
//		ground->SetPosition(2782 + 32 * 83 + i * 32.0f, 374);
//		objects.push_back(ground);
//	}
//	
//}

void Update(DWORD dt)
{
	float x, y;
	captain->GetPosition(x, y);
#pragma region Resource
	//if (lv1 == true)
	//{
	//	Item *bonus = NULL;
	//	// Thưởng bonus vòng 1
	//	if (captain->isBonus)
	//	{
	//		bonus = new Item();
	//		bonus->SetPosition(1367, 407);
	//		bonus->AddAnimation(ITEM_MONEY);
	//		bonus->SetType(ITEM_MONEY);
	//		captain->isBonus = false;
	//	}

	//	if (bonus != NULL)
	//	{
	//		if (bonus->y > 370)
	//		{
	//			bonus->y -= SIMON_CLIMBING_SPEED_Y * dt;
	//			bonus->Render();
	//		}
	//		else
	//		{
	//			objects.push_back(bonus);
	//		}
	//	}

	//	// Lên cấp
	//	/*if (captain->isLevelUp) {
	//		captain->SetState(SIMON_STATE_WALK);
	//		captain->SetSpeed(SIMON_LEVELUP_SPEED, 0);
	//		DWORD timer = GetTickCount();
	//		if (timer - captain->onCheckPointTime > LEVELUP_TIME)
	//		{
	//			for (int i = objects.size() - 1; i > 0; i--)
	//				objects.pop_back();

	//			lv2 = true;
	//			lv1 = false;
	//			captain->isLevelUp = false;
	//			captain->SetState(SIMON_STATE_IDLE);
	//		}
	//	}*/
	//}
	//if (lv2 == true)
	//{
	//	if (countLoadResourceLv2 == false)
	//	{
	//		game->x_cam = 0;
	//		LoadResourceLv2();
	//		countLoadResourceLv2 = true;
	//		captain->SetPosition(50, 155);
	//		timer = GetTickCount();
	//	}
	//	else if(countLoadResourceLv2 == true && x < MAX_WIDTH_LV2 - 2*SIMON_STAND_BBOX_WIDTH)
	//	{
	//		if (GetTickCount() - timer > 5000)
	//		{
	//			Zombie *zombie = new Zombie();
	//			zombie->AddAnimation(602);
	//			zombie->AddAnimation(604);
	//			zombie->SetPosition(1200, 376);
	//			zombie->SetState(ZOMBIE_STATE_WALKING);
	//			objects.push_back(zombie);
	//			timer = timer + 5000;
	//		}
	//	}
	//	else  //check point
	//	{
	//		lv2_1 = true;
	//		lv2 = false;
	//	}
	//}
	//if (lv2_1 == true)
	//{
	//	
	//	if (countLoadResourceLv2_1 == false)
	//	{
	//		for (int i = objects.size()-1; i > 0; i--)
	//			objects.pop_back();			
	//		LoadResourceLv2_1();
	//		countLoadResourceLv2_1 = true;
	//	}
	//	 if (x > MAX_WIDTH_LV2_1 - 2*SIMON_STAND_BBOX_WIDTH)
	//	{
	//		lv2_1 = false;			
	//		boss = true;

	//	}
	//	
	//}
	//if (lv2_2 == true)
	//{
	//	if (countLoadResourceLv2_2 == false)
	//	{
	//		for (int i = objects.size() - 1; i > 200; i--)
	//			objects.pop_back();
	//		countLoadResourceLv2_2 = true;
	//		captain->SetPosition(50, 150);
	//	}
	//}
	//if (boss == true)
	//{
	//	if (countLoadResourceboss == false)
	//	{
	//		for (int i = objects.size() - 1; i > 0; i--)
	//			objects.pop_back();
	//		LoadResourceboss();
	//		countLoadResourceboss = true;
	//		timer2 = GetTickCount();
	//	}
	//	else if(countLoadResourceboss == true)
	//	{
	//		if (GetTickCount() - timer2 > 5000)
	//		{
	//			Zombie *zombie = new Zombie();
	//			zombie->AddAnimation(602);
	//			zombie->AddAnimation(604);
	//			zombie->SetPosition(4900, 376);
	//			zombie->SetState(ZOMBIE_STATE_WALKING);
	//			objects.push_back(zombie);
	//			timer2 = timer2 + 5000;
	//		}
	//	}
	//	
	//}

#pragma endregion

#pragma region Collision
	vector<LPGAMEOBJECT> coObjects;
	if (captain->x < 0)
		currentGrids = listGrids->GetCurrentGrids(0);
	else
		currentGrids = listGrids->GetCurrentGrids(captain->x);

	for (int i = 0; i < currentGrids.size(); i++)
	{
		vector<LPGAMEOBJECT> listObjects = currentGrids[i]->GetListObject();
		int listObjectSize = listObjects.size();
		for (int j = 0; j < listObjectSize; j++)
		{
			coObjects.push_back(listObjects[j]);
		}
	}

	captain->Update(dt, &coObjects);
	if(boss == false)
		for (int i = 0; i < coObjects.size(); i++)
		{
			coObjects[i]->Update( dt, &coObjects);
		}
	else if(boss == true)
	{
		for (int i = 0; i < coObjects.size(); i++)
		{
				coObjects[i]->Update( dt, &coObjects);
				listGrids->UpdateObjectInGrid(coObjects[i]);
		}
		

	}

	
#pragma endregion	

#pragma region Remove Object
	vector<LPGAMEOBJECT> listRemoveObjects;
	for (int i = 0; i < coObjects.size(); i++)
	{
		if (dynamic_cast<Zombie *>(coObjects.at(i)))
		{
			Zombie *zombie = dynamic_cast<Zombie *>(coObjects.at(i));

			if (zombie->GetState() == ZOMBIE_STATE_DIE)
			{
				listRemoveObjects.push_back(zombie);
				item = new Item();
				item->SetPosition(zombie->x, zombie->y);
				item->SetSpeed(0, -0.1);
				coObjects.push_back(item);
				listGrids->AddObject(item);
				srand(time(NULL));
				int random_portion = rand() % 100;

				// Heart
				if (random_portion < 30)
				{
					item->AddAnimation(ITEM_HEART);
					item->SetType(ITEM_HEART);
				}
				// Item 1
				else if (random_portion >= 30 && random_portion < 50)
				{
					item->AddAnimation(ITEM_1);
					item->SetType(ITEM_1);
				}
				// Item 2
				else if (random_portion >= 50 && random_portion < 80)
				{
					item->AddAnimation(ITEM_2);
					item->SetType(ITEM_2);
				}
				// Item 3
				else
				{
					item->AddAnimation(ITEM_3);
					item->SetType(ITEM_3);
				}
			}
		}
		else if (dynamic_cast<Soldier *>(coObjects.at(i)))
		{
			Soldier *soldier = dynamic_cast<Soldier *>(coObjects.at(i));

			if (soldier->GetState() == SOLDIER_STATE_DIE)
			{
				listRemoveObjects.push_back(soldier);
				item = new Item();
				item->SetPosition(soldier->x, soldier->y);
				item->SetSpeed(0, -0.1);
				coObjects.push_back(item);
				listGrids->AddObject(item);
				srand(time(NULL));
				int random_portion = rand() % 100;

				// Heart
				if (random_portion < 30)
				{
					item->AddAnimation(ITEM_HEART);
					item->SetType(ITEM_HEART);
				}
				// Item 1
				else if (random_portion >= 30 && random_portion < 50)
				{
					item->AddAnimation(ITEM_1);
					item->SetType(ITEM_1);
				}
				// Item 2
				else if (random_portion >= 50 && random_portion < 80)
				{
					item->AddAnimation(ITEM_2);
					item->SetType(ITEM_2);
				}
				// Item 3
				else
				{
					item->AddAnimation(ITEM_3);
					item->SetType(ITEM_3);
				}
			}
		}

		else if (dynamic_cast<Lantern *>(coObjects.at(i)))
		{
			Lantern *lantern = dynamic_cast<Lantern *>(coObjects.at(i));
			if (lantern->isHitted)
			{
				listRemoveObjects.push_back(lantern);
				float lantern_x, lantern_y, lantern_right, lantern_bottom;
				lantern->GetBoundingBox(lantern_x, lantern_y, lantern_right, lantern_bottom);

				item = new Item();
				item->SetPosition(lantern_x, lantern_y);
				item->SetSpeed(0, -0.1);
				coObjects.push_back(item);
				listGrids->AddObject(item);
				// Whip item
				if (captain->whip->level < 2)
				{
					item->AddAnimation(ITEM_2);
					item->SetType(ITEM_2);
				}
				else
				{
					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 30)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Item 1
					else if (random_portion >= 30 && random_portion < 50)
					{
						item->AddAnimation(ITEM_1);
						item->SetType(ITEM_1);
					}
					// Item 2
					else if (random_portion >= 50 && random_portion < 80)
					{
						item->AddAnimation(ITEM_2);
						item->SetType(ITEM_2);
					}
					// Item 3
					else
					{
						item->AddAnimation(ITEM_3);
						item->SetType(ITEM_3);
					}
				}

			}
		}
		else if (dynamic_cast<Item *>(coObjects.at(i)))
		{
			Item *item = dynamic_cast<Item *>(coObjects.at(i));

			if (item->GetEaten())
			{
				//objects.erase(objects.begin() + i);
				//delete item;
				listRemoveObjects.push_back(item);
			}
		}
		else if (dynamic_cast<Effect *>(coObjects.at(i)))
		{
			Effect *effect = dynamic_cast<Effect *>(coObjects.at(i));

			if (effect->GetExposed())
			{
				//objects.erase(objects.begin() + i);
				//delete effect;
				listRemoveObjects.push_back(effect);
			}
		}
	}

	// Remove lần lượt từng object từ listRemoveObjects trong listGrids
	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		listGrids->RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
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
	/*else if( lv2 == true)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			game->y_cam = 0;
		}
		else if (x > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) {
			game->x_cam = MAX_WIDTH_LV2 - SCREEN_WIDTH;
			game->y_cam = 0;
		}
		else if(x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
			game->y_cam = 0;
		}

	}*/
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
	//			captain->SetState(SIMON_STATE_WALK);
	//			
	//		}
	//		if (x >3200 && x<3210 ) {
	//			check1 = true;
	//			check = true;
	//			captain->SetState(SIMON_STATE_IDLE);
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
	//	//trả camera về captain
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
	/*else if (lv2_2 == true)
	{
		game->x_cam = 0;
		game->y_cam = 0;
	}
	else if (boss == true)
	{
		if (game->x_cam < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
		{
			
				game->x_cam += SIMON_WALKING_SPEED * dt;
				game->y_cam = 0;
			

		}
		else {

			if (x < MAX_WIDTH_LV2_1 + 100)
			{
				if(check3 == false)
				captain->SetState(SIMON_STATE_WALK);
			}
			else if (x > MAX_WIDTH_LV2_1 + 100 && x < MAX_WIDTH_LV2_1 + 105)
			{
				captain->SetState(SIMON_STATE_IDLE);
				checkScene1 = true;
				check3 = true;
			}
		}
		
		if (checkScene1 == true)
		{
			if(game->x_cam < MAX_WIDTH_LV2_1)
				game->x_cam += SIMON_WALKING_SPEED * dt;
			else 
			{
				if (x > MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
				{
					game->x_cam = x - SCREEN_WIDTH / 2;
					game->y_cam = 0;
				}
				else if (x > MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2) {
					game->x_cam = MAX_WIDTH_LV2_2 - SCREEN_WIDTH;
					game->y_cam = 0;
				}
				else if (x < MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2)
				{
					game->x_cam = MAX_WIDTH_LV2_1;
					game->y_cam = 0;
				}
			}
		}
	}*/
#pragma endregion

#pragma region UI
	/*if (lv1 == true)
		ui->Update(gameTime/1000, 1,captain);
	else
		ui->Update(gameTime/1000, 2,captain);*/
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
		captain->GetPosition(x, y);

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
		for (int i = 0; i < currentGrids.size(); i++)
		{
			vector<LPGAMEOBJECT> listObject = currentGrids[i]->GetListObject();
			int listObjectSize = listObject.size();

			for (int j = 0; j < listObjectSize; j++)
			{
				listObject[j]->Render();
			}
		}

		captain->Render();
		//ui->Render(game->x_cam,game->y_cam,captain);
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
	
	listGrids = ListGrids::GetInstance();

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}