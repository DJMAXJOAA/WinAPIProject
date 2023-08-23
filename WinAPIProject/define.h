#pragma once

// ½Ì±ÛÅæ °´Ã¼ ºÒ·¯¿À±â
#define SINGLE(type) public:\
static type* GetInstance()\
{\
static type mgr;\
return &mgr;\
}

#define fDT CTimeMgr::GetInstance()->GetfDT()
#define DT CTimeMgr::GetInstance()->GetDT()

#define CLONE(type) type* Clone() {return new type(*this);}

#define KEY_CHECK(key, state) CKeyMgr::GetInstance()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)	
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)	
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInstance()->GetMousePos()

#define PI 3.1415926535f

// Áöµµ Å©±â
#define WIDTH 15
#define HEIGHT 7
#define TILE_WIDTH 140
#define TILE_HEIGHT 70
#define GRID_X 9
#define GRID_Y 9
#define DEBUG1(key) cout<<key<<"\n"
#define DEBUG2(key1, key2) cout<<key1<<","<<key2<<"\n"
#define DEBUG3(key1, key2, key3) cout<<key1<<","<<key2<<","<<key3<<"\n"

enum class GROUP_TYPE
{
	DEFAULT,
	BLOCK,
	TILE,
	MONSTER,
	PLAYER,
	MISSILE_PLAYER,
	MISSILE_MONSTER,
	MOUSE,

	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	BATTLE,

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,

	TILESELECT_TRIGGER,

	END,
};

enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,
};

enum class TILE_STATE
{
	BLACK,
	RED,
	GREEN,
	BLUE,
	PURPLE,
	RED_SELECTED,
	GREEN_SELECTED,
	BLUE_SELECTED,
	PURPLE_SELECTED,
};

enum class MAP_TYPE
{
	BRICK,
	DIRT,
	GRASS,
	STONE,
	SNOW,
};

enum class ATTACK_TYPE
{
	NORMAL_ATTACK,
};