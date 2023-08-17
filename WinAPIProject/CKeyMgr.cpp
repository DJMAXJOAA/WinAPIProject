#include "pch.h"
#include "CKeyMgr.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST]
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',
	VK_MENU,
	VK_LSHIFT,
	VK_SPACE,
	VK_CONTROL,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,
};

void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
	//m_vecKey[(int)KEY::LEFT].eState;
	//m_vecKey[(int)KEY::LEFT].bPrev; 이런시긍로 확인
}

void CKeyMgr::Update()
{
	// 윈도우 활성시에만 키입력 상태
	HWND hMainWnd = CCore::GetInstance()->GetMainhWnd();
	HWND hWnd = GetFocus();

	/*if (hWnd == hMainWnd)*/
	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// 키가 눌려있음
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// 이전에도 눌리고 있었음
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				// 이번에 새로 눌림
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}

			// 키가 안눌러져있음
			else
			{
				// 이전엔 눌려져 있었음(지금 뗐음)
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				// 이전에도 떼져있었음
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		}
		// 마우스 위치 계산, 전체 윈도우 기준이라 추가로 보정해줘야됨
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInstance()->GetMainhWnd(), &ptPos); // 현재 기준 윈도우로 보정

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// 윈도우 포커싱 해제 상태
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;
			// 눌렀거나 누르고 있는 상태면 자동으로 떼지게 하기
			if (KEY_STATE::TAP == m_vecKey[i].eState
				|| KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			// 떼지고 있는 상태라면, NONE으로 가게 하기
			if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}