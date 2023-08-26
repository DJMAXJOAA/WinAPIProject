#pragma once

// 싱글톤 객체 불러오기
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

#define DEBUG1(key) cout<<key<<"\n"
#define DEBUG2(key1, key2) cout<<key1<<","<<key2<<"\n"
#define DEBUG3(key1, key2, key3) cout<<key1<<","<<key2<<","<<key3<<"\n"

namespace battle
{
	// 지도 크기
#define WIDTH 15
#define HEIGHT 7
#define TILE_WIDTH 140
#define TILE_HEIGHT 70
#define GRID_X 9
#define GRID_Y 9

// 매크로 (좌표 변경)
#define REAL(key) m_TileCenter->GridToReal(key)
#define GRID(key) m_TileCenter->RealToGrid(key) 
}

enum class GROUP_TYPE
{
	DEFAULT,
	BLOCK,
	TILE,
	MISSILE_PLAYER,
	MISSILE_MONSTER,
	UNIT,
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
	TURN_CHANGE,

	ANIMATION_EVENT,
	ANIMATION_END,

	END,
};

enum class GRADE
{
	COMMON,
	RARE,
	EPIC,
	UNIQUE,
};

enum class ITEM_TYPE
{
	ARMOR,
	BOOTS,
	BOTTOM,
	GLOVES,
	HELMET,

	AXE = 10,
	BOW,
	SPEAR,
	STAFF,
	SWORD,
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

enum class ATTACK_TYPE
{
	NORMAL_ATTACK,
};

enum class FIELD_TYPE
{
	NONE,
	COMMON = 4,
	ELITE,
	SHOP,
	SHELTER,
	BOSS,
};

enum class BLOCK_TYPE
{
	BRICK,
	DIRT,
	GRASS,
	STONE,
	SNOW,
};

enum class TURN_TYPE
{
	ENTER,                  // 전투 입장
	PLAYER_START,           // 플레이어 :: 첫 블럭 선택 전
	PLAYER_TILESELECT,     // 플레이어 :: 블럭 선택중
	PLAYER_MOVE,            // 플레이어 :: 선택 확정하고, 움직이는중
	PLAYER_ATTACK,          // 플레이어 :: 이동중에 공격 상태 들어감(잠깐)
	PLAYER_SKILL,           // 플레이어 :: 움직임이 끝나고, 스킬 시전중
	ENEMY_MOVE,             // 적 :: 움직임 계산 + 움직임
	ENEMY_ATTACK,           // 적 :: 움직임이 끝나고, 플레이어 공격 혹은 스킬
	EXIT,                   // 전투 종료
};

enum class DIRECTION
{
	// BFS에서 사용
	FOUR_WAY,       // 상하좌우 방향
	DIAGONAL,       // 대각선 방향
	EIGHT_WAY,      // 8방향 전부
};

enum class MONSTER_STRATEGY
{
	TRACE,
	RANDOM_MOVE,
	SELF_HEAL,
	RANDOM_TELEPORT,
	SELF_BARRIER,
	FRIENDLY_HEAL,
};