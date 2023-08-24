#include "pch.h"
#include "CTurnManager.h"

#include "CTile.h"

void CTurnManager::BlockSelectInit(CObject* _pObj, Vec2 );
{
	CTile* tile = (CTile*)_pObj;

	// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�, �� ��ȯ
	m_TileColor = tile->pTile->GetTileState();
	m_vSelectTile = selectPos;
	m_lstTile.push_back(selectPos);
	tile->SetTileState((TILE_STATE)((int)tile->pTile->GetTileState() + 4));

	// �����
	DEBUG2(selectPos.x, selectPos.y);
	DEBUG1((int)m_TileColor);
	printf("Ÿ�� ����\n");
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
		// Ÿ�� ����
		if (KEY_TAP(KEY::CTRL))
		{
			PlayerMoveInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
		// ���� ���
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
		// BFS Ž�� ����� ���� �����ϰ� �Ǹ�, ��� ���� ���¿� ���� �� �ٽ� �̵����� ����
		if (!m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_ATTACK;
			break;
		}

		// �̵��� Ÿ���� �������, ��ų �ܰ�� �Ѿ�Ե�
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
		// ���� ó���� ���� ó���ϸ�, �ٽ� �����̴� ���·� ��ȯ��Ű��
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
	ChangedTurn(_type);		// ������ �̺�Ʈ ����
}
