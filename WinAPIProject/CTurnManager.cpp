#include "pch.h"
#include "CTurnManager.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"

#include "CTile.h"

CTurnManager::CTurnManager()
	: m_CurrentTurn(TURN_TYPE::PLAYER_START)
	, m_lstMoveRoute{}
	, m_lstTargetEnemy{}
{
}

CTurnManager::~CTurnManager()
{
}

void CTurnManager::ChangeTurn(TURN_TYPE _type)
{
	// ���� Ÿ�� ����
	m_CurrentTurn = _type;

	// ������ �̺�Ʈ ����(�� �������� -> Init����)
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TURN_CHANGE;
	evn.lParam = (DWORD_PTR)_type;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CTurnManager::Update()
{
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::ENTER: break;
	case TURN_TYPE::PLAYER_START: break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		// Ÿ�� ����
		if (KEY_TAP(KEY::CTRL))
		{
			ChangeTurn(TURN_TYPE::PLAYER_MOVE);
			break;
		}
		// ���� ���
		if (KEY_TAP(KEY::ESC))
		{
			ChangeTurn(TURN_TYPE::PLAYER_START);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// BFS Ž�� ����� ���� �����ϰ� �Ǹ�, ��� ���� ���¿� ���� �� �ٽ� �̵����� ����
		if (!m_lstTargetEnemy.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
			break;
		}

		// �̵��� Ÿ���� �������, ��ų �ܰ�� �Ѿ�Ե�
		if (m_lstMoveRoute.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_SKILL);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// ���� ó���� ���� ó���ϸ�, �ٽ� �����̴� ���·� ��ȯ��Ű��
		if (m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_SKILL:
	{
		m_CurrentTurn = TURN_TYPE::PLAYER_START;
	}
	break;
	case TURN_TYPE::ENEMY_MOVE:
		break;
	case TURN_TYPE::ENEMY_ATTACK:
		break;
	case TURN_TYPE::EXIT:
		break;
	default:
		break;
	}
}

void CTurnManager::Init()
{
	m_CurrentTurn = TURN_TYPE::PLAYER_START;
	m_lstMoveRoute.clear();
	m_lstTargetEnemy.clear();
}