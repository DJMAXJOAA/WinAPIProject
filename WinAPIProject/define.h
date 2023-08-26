#pragma once

// �̱��� ��ü �ҷ�����
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
	// ���� ũ��
#define WIDTH 15
#define HEIGHT 7
#define TILE_WIDTH 140
#define TILE_HEIGHT 70
#define GRID_X 9
#define GRID_Y 9

// ��ũ�� (��ǥ ����)
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
	ENTER,                  // ���� ����
	PLAYER_START,           // �÷��̾� :: ù �� ���� ��
	PLAYER_TILESELECT,     // �÷��̾� :: �� ������
	PLAYER_MOVE,            // �÷��̾� :: ���� Ȯ���ϰ�, �����̴���
	PLAYER_ATTACK,          // �÷��̾� :: �̵��߿� ���� ���� ��(���)
	PLAYER_SKILL,           // �÷��̾� :: �������� ������, ��ų ������
	ENEMY_MOVE,             // �� :: ������ ��� + ������
	ENEMY_ATTACK,           // �� :: �������� ������, �÷��̾� ���� Ȥ�� ��ų
	EXIT,                   // ���� ����
};

enum class DIRECTION
{
	// BFS���� ���
	FOUR_WAY,       // �����¿� ����
	DIAGONAL,       // �밢�� ����
	EIGHT_WAY,      // 8���� ����
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