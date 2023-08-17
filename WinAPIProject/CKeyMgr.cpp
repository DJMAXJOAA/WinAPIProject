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
	//m_vecKey[(int)KEY::LEFT].bPrev; �̷��ñ�� Ȯ��
}

void CKeyMgr::Update()
{
	// ������ Ȱ���ÿ��� Ű�Է� ����
	HWND hMainWnd = CCore::GetInstance()->GetMainhWnd();
	HWND hWnd = GetFocus();

	/*if (hWnd == hMainWnd)*/
	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// Ű�� ��������
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// �������� ������ �־���
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				// �̹��� ���� ����
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}

			// Ű�� �ȴ���������
			else
			{
				// ������ ������ �־���(���� ����)
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				// �������� �����־���
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		}
		// ���콺 ��ġ ���, ��ü ������ �����̶� �߰��� ��������ߵ�
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInstance()->GetMainhWnd(), &ptPos); // ���� ���� ������� ����

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// ������ ��Ŀ�� ���� ����
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;
			// �����ų� ������ �ִ� ���¸� �ڵ����� ������ �ϱ�
			if (KEY_STATE::TAP == m_vecKey[i].eState
				|| KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			// ������ �ִ� ���¶��, NONE���� ���� �ϱ�
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