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

enum class GROUP_TYPE
{
	DEFAULT,
	MONSTER,
	PLAYER,
	MISSILE_PLAYER,
	MISSILE_MONSTER,
	TILE,

	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,

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