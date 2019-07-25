//file path
#define GAME_FONT L"Resources\\prstart.ttf"

#define SIMON_WALKING_SPEED				0.15f 
#define SIMON_CLIMBING_SPEED_X			0.05f
#define SIMON_CLIMBING_SPEED_Y			0.05f
#define SIMON_JUMP_SPEED_Y				0.52f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_LEVELUP_SPEED				0.01f
#define SIMON_GRAVITY					0.0015f
#define SIMON_DIE_DEFLECT_SPEED			0.5f
#define SIMON_SIT_TO_STAND				15
#define SIMON_ON_LEFT_CHECKSTAIR		1
#define SIMON_ON_RIGHT_CHECKSTAIR		2
#define SIMON_ONSTAIR_ERR_RANGE			3

#define SIMON_DISTANCE_DASH				250
//state
#define SIMON_STATE_IDLE				0
#define SIMON_STATE_UNTOUCHABLE			100
#define SIMON_STATE_DIE					200
#define SIMON_STATE_ONCHECKSTAIR		300
#define SIMON_STATE_SIT					500
#define SIMON_STATE_WALK				600
#define SIMON_STATE_ONSTAIR				700
#define SIMON_STATE_ONSTAIR_IDLE		800
#define SIMON_STATE_HOLD_SHIELD			900
#define SIMON_STATE_DASHING				1000
#define SIMON_STATE_DOUBLE_JUMP			1100

//action
#define SIMON_ACTION_JUMP				300
#define SIMON_ACTION_ATTACK				600

//animation
#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3
#define SIMON_ANI_ATTACK_RIGHT			4
#define SIMON_ANI_ATTACK_LEFT			5
#define SIMON_ANI_SIT_RIGHT				6
#define SIMON_ANI_SIT_LEFT				7
#define SIMON_ANI_JUMP_RIGHT			8
#define SIMON_ANI_JUMP_LEFT				9
#define SIMON_ANI_SIT_ATTACK_RIGHT		10
#define SIMON_ANI_SIT_ATTACK_LEFT		11
#define SIMON_ANI_HITUP_RLADDER			12 // hit right on stair which has up-direction from left to right
#define SIMON_ANI_HITDOWN_LLADDER		13  
#define SIMON_ANI_HITDOWN_RLADDER		14 //hit right on stair which has up-direction from right to left 
#define SIMON_ANI_HITUP_LLADDER			15 //use dik_ and nx to select ani
#define SIMON_ANI_WALKUP_RLADDER		16 // walk up on stair which has up-direction from left to right
#define SIMON_ANI_WALKDOWN_LLADDER		17 
#define SIMON_ANI_WALKUP_LLADDER		18
#define SIMON_ANI_WALKDOWN_RLADDER		19
#define SIMON_ANI_IDLE_WALKUP_RLADDER	20
#define SIMON_ANI_IDLE_WALKDOWN_LLADDER	21
#define SIMON_ANI_IDLE_WALKUP_LLADDER	22
#define SIMON_ANI_IDLE_WALKDOWN_RLADDER	23
#define SIMON_ANI_HURT_LEFT				24
#define SIMON_ANI_HURT_RIGHT			25
#define SIMON_ANI_DIE					26
#define SIMON_ANI_JUMP_ATTACK_RIGHT		27
#define SIMON_ANI_JUMP_ATTACK_LEFT		28
#define SIMON_ANI_DOUBLE_JUMP			29
#define SIMON_ANI_DASHING_RIGHT			30
#define SIMON_ANI_DASHING_LEFT			31
#define SIMON_ANI_SHIELD_RIGHT			32
#define SIMON_ANI_SHIELD_LEFT			33
// Whip
#define WHIP_RIGHT 						0
#define WHIP_LEFT						1
#define WHIP_RIGHT_1					2
#define WHIP_LEFT_1						3
#define WHIP_RIGHT_2					4
#define WHIP_LEFT_2						5


#define WHIP_LV0_BBOX_WIDTH				100
#define WHIP_LV0_BBOX_HEIGHT			15
#define WHIP_LV1_BBOX_WIDTH				105
#define WHIP_LV1_BBOX_HEIGHT			15
#define WHIP_LV2_BBOX_WIDTH				125
#define WHIP_LV2_BBOX_HEIGHT			15

// Big fire
#define BIGFIRE_BBOX_WIDTH				32
#define BIGFIRE_BBOX_HEIGHT				64

// Lantern 
#define LANTERN_BBOX_WIDTH				16
#define LANTERN_BBOX_HEIGHT				16


// Heart
#define HEART_BBOX_WIDTH				16
#define HEART_BBOX_HEIGHT				16

// Whip item
#define WHIPITEM_BBOX_WIDTH				32
#define WHIPITEM_BBOX_HEIGHT			32

//Knife
#define KNIFE_BBOX_WIDTH				32
#define KNIFE_BBOX_HEIGHT				18

//Money
#define MONEY_BBOX_WIDTH				30
#define MONEY_BBOX_HEIGHT				30

// Zombie
#define ZOMBIE_WALKING_SPEED			0.07f

#define ZOMBIE_BBOX_WIDTH				32
#define ZOMBIE_BBOX_HEIGHT				32
#define ZOMBIE_BBOX_HEIGHT_DIE			9

#define ZOMBIE_STATE_WALKING			100
#define ZOMBIE_STATE_DIE				200

#define ZOMBIE_ANI_WALKING_LEFT			0
#define ZOMBIE_ANI_WALKING_RIGHT		1
#define ZOMBIE_ANI_DIE					2

#define PANTHER_WALKING_SPEED			0.16f
//Panther
#define PANTHER_BBOX_WIDTH				32
#define PANTHER_BBOX_HEIGHT				32
#define PANTHER_BBOX_HEIGHT_DIE			9

#define PANTHER_STATE_WAIT				100
#define PANTHER_STATE_ATTACK			150
#define PANTHER_STATE_DIE				200


#define PANTHER_ANI_WAIT				0
#define PANTHER_ANI_RUN_LEFT			1
#define PANTHER_ANI_RUN_RIGHT			2
#define PANTHER_ANI_GRAB				3
#define PANTHER_ANI_DIE					4

//BossBat
#define BOSSBAT_BBOX_WIDTH				100	
#define BOSSBAT_BBOX_HEIGHT				45

#define BOSSBAT_STATE_WAIT				500
#define BOSSBAT_STATE_ATTACK			600
#define BOSSBAT_STATE_DIE				700

#define BOSSBAT_ANI_WAIT				0
#define BOSSBAT_ANI_ATTACK				1

// Check stair
#define CHECKSTAIR_UP_LEFT				0
#define CHECKSTAIR_UP_RIGHT				1
#define CHECKSTAIR_DOWN_LEFT			2
#define CHECKSTAIR_DOWN_RIGHT			3

#define CHECKSTAIR_BBOX_WIDTH			32
#define CHECKSTAIR_BBOX_HEIGHT			32

// Timer
#define SIMON_TIMER_ATTACK				450

#define	SIMON_LEVEL_SMALL				1
#define	SIMON_LEVEL_BIG					2

#define SIMON_STAND_BBOX_WIDTH			32
#define SIMON_STAND_BBOX_HEIGHT			45
#define SIMON_SIT_BBOX_WIDTH			32
#define SIMON_SIT_BBOX_HEIGHT			47

#define SIMON_UNTOUCHABLE_TIME			5000

#define WINDOW_CLASS_NAME L"Game Captain America"
#define MAIN_WINDOW_TITLE L"Game Captain America"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH					640
#define SCREEN_HEIGHT					328
#define MAX_WIDTH_LV1					2048
#define MAX_WIDTH_LV2					3085
#define MAX_WIDTH_LV2_1					4096
#define MAX_WIDTH_LV2_2					5650
#define MAX_FRAME_RATE					120

//texture
#define ID_TEX_MARIO					0
#define ID_TEX_LV1						10
#define ID_TEX_LV1_2					15
#define ID_TEX_BRICK					20
#define ID_TEX_MARIO_2					30
#define ID_TEX_PANTHER					25
#define ID_TEX_PANTHER_RIGHT			26
#define ID_TEX_ZOMBIE					35
#define ID_TEX_ZOMBIE_RIGHT				36
#define ID_TEX_FIRE						40
#define ID_TEX_CANDLE					45
#define ID_TEX_WHIP						50
#define ID_TEX_WHIP_2					55
#define ID_TEX_TILESET					60
#define ID_TEX_TILESET_2				61
#define ID_TEX_TILESET_3				62
#define ID_TEX_BRICK2					65
#define ID_TEX_LADDER					70
#define ID_TEX_LADDER_LEFT				75
#define ID_TEX_STAIR_BOT				80
#define ID_TEX_STAIR_TOP				85
#define ID_TEX_MERMAN_RIGHT				90
#define ID_TEX_MERMAN_LEFT				95
#define ID_TEX_BAT                      100 
#define ID_TEX_HEART					105
#define ID_TEX_BOSS						110
#define ID_TEX_SIT						200

#define ID_TEX_AXE						115
#define ID_TEX_BLACK					120
#define ID_TEX_CROSS					125
#define ID_TEX_DOUBLE_SHOT_UI			130
#define ID_TEX_DOUBLE_SHOT				135
#define ID_TEX_TRIPLE_SHOT_UI			140
#define ID_TEX_TRIPLE_SHOT				145
#define ID_TEX_ENEMYHP					150
#define ID_TEX_HOLY_WATER				155
#define ID_TEX_HP						160
#define ID_TEX_KNIFE					165
#define ID_TEX_NOHP						170
#define ID_TEX_STOP_WATCH				175
#define ID_TEX_RECT						176

#define ID_TEX_EFFECT1					180
#define ID_TEX_EFFECT2					185
#define ID_TEX_WHIP_VIP					190
#define ID_TEX_MONEY					195
#define ID_TEX_LANTERN					196

#define BRICK_BBOX_WIDTH				16
#define BRICK_BBOX_HEIGHT				16

#define EFFECT_TIME						200
#define LEVELUP_TIME					3000

// Item
#define ITEM_WHIPITEM					808
#define ITEM_HEART						805
#define ITEM_KNIFE						809
#define ITEM_MONEY						810

// Checkpoint type
#define CHECKPOINT_LEVELUP				0
#define CHECKPOINT_BONUS				1