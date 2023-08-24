#include "pch.h"
#include "CTurnManager.h"

#include "CTile.h"

void CTurnManager::BlockSelectInit(CObject* _pObj, Vec2 );
{
	CTile* tile = (CTile*)_pObj;

	// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기, 턴 전환
	m_TileColor = tile->pTile->GetTileState();
	m_vSelectTile = selectPos;
	m_lstTile.push_back(selectPos);
	tile->SetTileState((TILE_STATE)((int)tile->pTile->GetTileState() + 4));

	// 디버깅
	DEBUG2(selectPos.x, selectPos.y);
	DEBUG1((int)m_TileColor);
	printf("타일 시작\n");
}

CTurnManager::CTurnManager(CScene_Battle* _pScene)
	: m_pScene(_pScene)
	, m_CurrentTurn(TURN_TYPE::PLAYER_START)
{
}

CTurnManager::~CTurnManager()
{
}

void CTurnManager::Update()
{
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::ENTER:
		break;
	case TURN_TYPE::PLAYER_START:
		break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		// 타일 결정
		if (KEY_TAP(KEY::CTRL))
		{
			PlayerMoveInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
		// 선택 취소
		if (KEY_TAP(KEY::ESC))
		{
			TileSelectInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_START;
			break;
		}
	}
	break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// BFS 탐색 결과로 적이 존재하게 되면, 잠깐 공격 상태에 돌입 후 다시 이동상태 돌입
		if (!m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_ATTACK;
			break;
		}

		// 이동할 타일이 사라지면, 스킬 단계로 넘어가게됨
		if (m_lstTile.empty())
		{
			PlayerSkillInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_SKILL;
			break;
		}
		PlayerMove();
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
		PlayerAttack();
	}
	break;
	case TURN_TYPE::PLAYER_SKILL:
	{
		TileSelectInit();
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

void CTurnManager::ChangeTurn(TURN_TYPE _type)
{
	m_CurrentTurn = _type;
	ChangedTurn(_type);		// 씬에게 이벤트 보냄
}
