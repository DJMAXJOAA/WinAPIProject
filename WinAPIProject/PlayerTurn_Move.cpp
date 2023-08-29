#include "pch.h"
#include "PlayerTurn_Move.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

void PlayerTurn_Move::Handle(CScene_Battle* _pScene)
{
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	BFSSearch* m_BFS = _pScene->GetBFS();
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	

	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	if (moveRoute.empty())
	{
		// �� �Ŵ������� ���� üũ �ؼ�, ���� �����
		return;
	}

	Vec2 vDestination = REAL(moveRoute.front());
	Vec2 vCurrentPos = m_pPlayer->GetPos();

	// ��������(��ġ�� ��ġ) = �ֺ� ���� üũ -> ����
	if (vCurrentPos == vDestination)
	{
		// BFS Ž�� ���� ��ġ ����
		m_TileCenter->SetTileObject(m_pPlayer->GetGridPos(), nullptr);
		m_TileCenter->SetTileObject(GRID(vDestination), m_pPlayer);

		// 1. ���� Ž�� ��, ������ ������ �Լ� ���� �� ���ݻ��� ��(lstTargetEnemy �迭�� ���� ���� ��)
		// Update���� �� ����Ʈ ��� üũ�ϰ�, �Ⱥ������ �Լ� ���� �� ���ݻ��·� ����
		vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
		list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
		m_BFS->BFS(GRID(vDestination), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 1);

		// ������ ������, ���� ���� ����
		if (!lstMonsters.empty())
		{
			m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
		}
		m_TileCenter->TileVisitedInit();

		// 2. Ÿ�� ���� ���� -> �̵��� ������ ������ ó��
		Vec2 gridDestination = moveRoute.front();
		vector<vector<TileState>>& tiles = m_TileCenter->GetTiles();
		tiles[(int)gridDestination.y][(int)gridDestination.x].pTile->SetTileState(TILE_STATE::BLACK);

		// 3. Ÿ�� ���� ����(������, ������ġ ����), Ÿ�� ����Ʈ ��ĭ ����		

		m_pPlayer->SetGridPos(moveRoute.front());
		moveRoute.pop_front();
		if (moveRoute.empty()) return;		// ������ Ÿ�� �����ϸ�, �Լ� Ż��

		vDestination = REAL(moveRoute.front());
	}

	// ĳ���Ϳ��� ������������ �����̰� ���
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), GRID(vDestination), m_pPlayer->GetPos(), vDestination);
	m_pPlayer->Move(gridDirection, vDestination);
}