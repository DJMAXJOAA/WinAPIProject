#include "pch.h"
#include "CTurnManager.h"

#include "CKeyMgr.h"

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

void CTurnManager::Update()
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
	}
	break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// BFS 탐색 결과로 적이 존재하게 되면, 잠깐 공격 상태에 돌입 후 다시 이동상태 돌입
		if (!m_lstTargetEnemy.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
			break;
		}

		// 이동할 타일이 사라지면, 스킬 단계로 넘어가게됨
		if (m_lstMoveRoute.empty())
		{
			ChangeTurn(TURN_TYPE::PLAYER_SKILL);
			break;
		}
		RunTurnLogic(TURN_TYPE::PLAYER_MOVE);
	}
	break;
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// 공격 처리를 전부 처리하면, 다시 움직이는 상태로 전환시키기
		if (m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
		RunTurnLogic(TURN_TYPE::PLAYER_ATTACK);
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