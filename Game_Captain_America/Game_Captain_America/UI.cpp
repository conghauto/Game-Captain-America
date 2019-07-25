#include "UI.h"

UI::UI()
{
	simonHP = NULL;
	simonHP = NULL;
}


UI::~UI()
{
	
}

bool UI::Initialize(LPDIRECT3DDEVICE9 d3ddv, Simon * simon)
{
	CTextures * textures = CTextures::GetInstance();	
	heart = new CSprite(1, 0, 0, 16, 16, textures->Get(ID_TEX_HEART));
	pinkrect = new CSprite(2, 65, 37 ,131 ,82, textures->Get(ID_TEX_RECT));
	axe = new CSprite(3, 0, 0, 30, 30, textures->Get(ID_TEX_AXE));
	holyWater = new CSprite(4, 0, 0, 30, 30, textures->Get(ID_TEX_HOLY_WATER));
	cross = new CSprite(5, 0, 0, 30, 30, textures->Get(ID_TEX_CROSS));
	knife = new CSprite(6, 0, 0, 30, 30, textures->Get(ID_TEX_KNIFE));
	stopWatch = new CSprite(7, 0, 0, 30, 30, textures->Get(ID_TEX_STOP_WATCH));

	
	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(8,0,0,8,15, textures->Get(ID_TEX_HP));
		
		simonHPList.push_back(sprite);
	}
	
	for (int i = 0; i < 16; i++)
	{		
		CSprite* sprite = new CSprite(8, 0, 0,8, 15, textures->Get(ID_TEX_ENEMYHP));
		enemyHPList.push_back(sprite);
	}

	font = NULL;
	AddFontResourceEx(GAME_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (!SUCCEEDED(result))
		return false;
	SetRect(&rect, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
	information = "SCORE_000000 TIME 0000 STAGE 00\n";
	information += "PLAYER                =62\n";
	information += "ENEMY                 P=3\n";
	return false;
}

//void UI::Update(int bossHP, int time, int life, int stage)
void UI::Update( int time, int stage,Simon *simon)
{
	this->time = time;
	this->stage = stage;


	std::string scoreString = std::to_string(simon->GetScore());
	while (scoreString.length() < 6)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->stage);
	while (stageString.length() < 2)
		stageString = "0" + stageString;

	information = "SCORE_" + scoreString +  " TIME " + timeString + " STAGE " + stageString + "\n";
	information += "PLAYER                =" + std::to_string(simon->GetHeart()) + "\n";
	information += "ENEMY                P=" + std::to_string(simon->GetLife()) + "\n";
}

void UI::Render(float x,float y,Simon *simon)
{	
	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	for (int i = 0; i < simonHPList.size(); i++)
	{
		simonHPList[i]->Draw(x + 130 + 11 * i, 42 , 255);
	}
	for (int i = 0; i < enemyHPList.size(); i++)
	{
		enemyHPList[i]->Draw(x + 130 + 11 * i, 62, 255);
	}
	//axe->Draw(x + SCREEN_WIDTH/2 + 20 , 60, 255);
	heart->Draw(x + SCREEN_WIDTH / 2 + 105 ,40, 255);
	pinkrect->Draw(x + SCREEN_WIDTH / 2 , 40, 255);
	if (simon->currentWeapon != NULL)
	{
		int weapon = simon->currentWeapon;
		{
			switch (weapon)
			{
			case ITEM_KNIFE:
				knife->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			}
		}
	}
}


