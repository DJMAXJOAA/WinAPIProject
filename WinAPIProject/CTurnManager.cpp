#include "pch.h"
#include "CTurnManager.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"

#include "CScene_Battle.h"
#include "CTile.h"

CTurnCenter::CTurnCenter()
	: m_CurrentTurn(TURN_TYPE::PLAYER_START)
	, m_lstMoveRoute{}
	, m_lstTarget{}
	, m_iCombo(0)
{
}

CTurnCenter::~CTurnCenter()
{
}

void CTurnCenter::ChangeTurn(TURN_TYPE _type)
{
	// ���� Ÿ�� ����
	m_CurrentTurn = _type;

	// ������ �̺�Ʈ ����(�� �������� -> ������ Init����)
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TURN_CHANGE;
	evn.lParam = (DWORD_PTR)_type;

	CEventMgr::GetInstance()->AddEventLate(evn);
}

void CTurnCenter::Update(CScene_Battle* _pScene)
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

		{
			_pScene->SetBattleState(TURN_TYPE::PLAYER_TILESELECT);
		}
	}
	break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// BFS Ž�� ����� ���� �����ϰ� �Ǹ�, ��� ���� ���¿� ���� �� �ٽ� �̵����� ����
		if (!m_lstTarget.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
			break;
		}

		// �̵��� Ÿ���� �������, ��ų �ܰ�� �Ѿ�Ե�
		else if (m_lstMoveRoute.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_SKILL);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// ���� ó���� ���� ó���ϸ�, �ٽ� �����̴� ���·� ��ȯ��Ű��
		if (m_lstTarget.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_MOVE);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_SKILL:
	{
		// ĳ������ ��ų ������ ������, ��Ʋ ������ �̺�Ʈ�� �޾Ƽ� ���� �Ѱ��ش�
		break;
	}
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

void CTurnCenter::Init()
{
	m_CurrentTurn = TURN_TYPE::PLAYER_START;
	m_lstMoveRoute.clear();
	m_lstTarget.clear();
}
