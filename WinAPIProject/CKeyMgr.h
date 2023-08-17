#pragma once

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	LSHIFT,
	SPACE,
	CTRL,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST // 끝
};

enum class KEY_STATE
{
	TAP,		// 막 누른 시점
	HOLD,		// 누르고 있는 중
	AWAY,		// 막 뗀 시점
	NONE,		// 눌리지 않음, 이전에도 눌리지 않은 상태
};

struct tKeyInfo
{
	KEY_STATE	eState;		// 키의 상태값(enum)
	bool		bPrevPush;	// 이전 프레임에 눌림?
};

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2			 m_vCurMousePos;

private:
	CKeyMgr();
	~CKeyMgr();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2 GetMousePos() { return m_vCurMousePos; }

public:
	void Init();
	void Update();

};