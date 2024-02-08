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
// ��ũ�� (��ǥ ����)
#define REAL(key) m_TileCenter->GridToReal(key)
#define GRID(key) m_TileCenter->RealToGrid(key)
}

struct ROBBY_SETTINGS
{
	// ���� GRID ũ��
	static const int WIDTH	= 15;
	static const int HEIGHT = 7;

	// ���� ��ġ, ũ��
	static const int WIDTH_MARGIN	= 50;
	static const int HEIGHT_MARGIN	= 50;
	static const int EXTEND_WIDTH	= (WIDTH * 2) - 1;
	static const int EXTEND_HEIGHT	= (HEIGHT * 2) - 1;
	static const int PANEL_WIDTH	= 2400;
	static const int PANEL_HEIGHT	= 640;

	// ���� ����
	static const int NUMBER_OF_ROUTE	= 6;	// ��Ʈ ����

	static const int ELITE_ROOM_CHANCE	= 15;	// ����Ʈ�� Ȯ��
	static const int SHOP_ROOM_CHANCE	= 25;	// ���� Ȯ��
	static const int REST_ROOM_CHANCE	= 40;	// �޽� Ȯ��
};

struct BATTLE_SETTINGS
{
	// ������ GRID ũ��
	static const int GRID_X	= 9;
	static const int GRID_Y	= 9;

	// Ÿ�� ũ��
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
	UP,		// 1�� ����
	RIGHT,	// 4�� ����
	DOWN,	// 7�� ����
	LEFT,	// 10�� ����
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
	ENTER,                  // ���� ����
	PLAYER_START,           // �÷��̾� :: ù �� ���� ��
	PLAYER_TILESELECT,     // �÷��̾� :: �� ������
	PLAYER_MOVE,            // �÷��̾� :: ���� Ȯ���ϰ�, �����̴���
	PLAYER_ATTACK,          // �÷��̾� :: �̵��߿� ���� ���� ��(���)
	PLAYER_SKILL,           // �÷��̾� :: �������� ������, ��ų ������
	ENEMY_MOVE,             // �� :: ������ ��� + ������
	WIN,                   // ���� ����
	DEFEAT,					// �й�
	
	END,
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

	NONE,
};

enum class SOUND_TYPE
{
	WIN,
	LOSE,

	END,
};
