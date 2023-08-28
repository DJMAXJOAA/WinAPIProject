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
	// 현재 타입 설정
	m_CurrentTurn = _type;

	// 씬에게 이벤트 보냄(턴 끝났따고 -> 씬에서 Init실행)
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
		// 타일 결정
		if (KEY_TAP(KEY::CTRL))
		{
			ChangeTurn(TURN_TYPE::PLAYER_MOVE);
			break;
		}
		// 선택 취소
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
		// BFS 탐색 결과로 적이 존재하게 되면, 잠깐 공격 상태에 돌입 후 다시 이동상태 돌입
		if (!m_lstTarget.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
			break;
		}

		// 이동할 타일이 사라지면, 스킬 단계로 넘어가게됨
		else if (m_lstMoveRoute.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_SKILL);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// 공격 처리를 전부 처리하면, 다시 움직이는 상태로 전환시키기
		if (m_lstTarget.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_MOVE);
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_SKILL:
	{
		// 캐릭터의 스킬 동작이 끝나면, 배틀 씬에서 이벤트를 받아서 턴을 넘겨준다
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
