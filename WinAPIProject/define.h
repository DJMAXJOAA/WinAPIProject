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
// 매크로 (좌표 변경)
#define REAL(key) m_TileCenter->GridToReal(key)
#define GRID(key) m_TileCenter->RealToGrid(key)
}

struct ROBBY_SETTINGS
{
	// 던전 GRID 크기
	static const int WIDTH	= 15;
	static const int HEIGHT = 7;

	// 생성 위치, 크기
	static const int WIDTH_MARGIN	= 50;
	static const int HEIGHT_MARGIN	= 50;
	static const int EXTEND_WIDTH	= (WIDTH * 2) - 1;
	static const int EXTEND_HEIGHT	= (HEIGHT * 2) - 1;
	static const int PANEL_WIDTH	= 2400;
	static const int PANEL_HEIGHT	= 640;

	// 던전 설정
	static const int NUMBER_OF_ROUTE	= 6;	// 루트 개수

	static const int ELITE_ROOM_CHANCE	= 15;	// 엘리트방 확률
	static const int SHOP_ROOM_CHANCE	= 25;	// 상점 확률
	static const int REST_ROOM_CHANCE	= 40;	// 휴식 확률
};

struct BATTLE_SETTINGS
{
	// 전투맵 GRID 크기
	static const int GRID_X	= 9;
	static const int GRID_Y	= 9;

	// 타일 크기
	static const int TILE_WIDTH		= 140;
	static const int TILE_HEIGHT	= 70;
};

enum class ROOM_TYPE
{
	NONE,

	UP_DIRECTION,
	RIGHT_DIRECTION,
	DOWN_DIRECTION,
	NORMAL_ROOM,
	ELITE_ROOM,
	SHOP_ROOM,
	REST_ROOM,
};

enum class GROUP_TYPE
{
	DEFAULT,
	BACKGROUND,
	BLOCK,
	TILE,
	UNIT,
	MOUSE,
	EFFECT,
	NUMBER,

	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	BATTLE,
	ROBBY,
	ENDING,

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

	GAME_START,
	GAME_LOAD,

	CAMERA_EVENT,

	TILESELECT_TRIGGER,
	TURN_CHANGE,

	FIELD_ENTER_STAGE,

	ANIMATION_EVENT,
	ANIMATION_END,

	PLAYER_ATTACK,
	PLAYER_ATTACK_DONE,
	PLAYER_SKILL_CAST,
	PLAYER_SKILL_DONE,

	MONSTER_ATTACK,
	MONSTER_ATTACK_DONE,

	MONSTER_DIED,
	PLAYER_DIED,

	END,
};

enum class GRID_DIRECTION
{
	UP,		// 1시 방향
	RIGHT,	// 4시 방향
	DOWN,	// 7시 방향
	LEFT,	// 10시 방향
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
	UP,
	MID,
	DOWN,
	COMMON = 4,
	ELITE,
	SHOP,
	SHELTER,
	BOSS,
	MESSAGE,
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
	WIN,                   // 전투 종료
	DEFEAT,					// 패배
	
	END,
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

	NONE,
};

enum class SOUND_TYPE
{
	WIN,
	LOSE,

	END,
};
