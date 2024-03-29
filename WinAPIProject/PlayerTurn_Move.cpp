#include "pch.h"
#include "PlayerTurn_Move.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

void PlayerTurn_Move::Init(CScene_Battle* _pScene)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// 타일 타겟 다시 지정
	m_TileCenter->SetTileObject(m_pPlayer->GetGridPos(), m_pPlayer);

	// 카메라 플레이어로 타겟 변경
	CCamera::GetInstance()->SetTarget(m_pPlayer);

	printf("CScene_Battle::TurnInit :: 플레이어 이동 시작 초기화\n");
}

void PlayerTurn_Move::Handle(CScene_Battle* _pScene)
{
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();

	// 이동할 타일이 사라지면, 스킬 단계로 넘어가게됨
	if (moveRoute.empty())
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_SKILL);
		return;
	}

	Vec2 vDestination = REAL(moveRoute.front());
	Vec2 vCurrentPos = m_pPlayer->GetPos();

	// 도착했음(위치가 일치) = 주변 적군 체크 -> 갱신
	if (vCurrentPos == vDestination)
	{
		// BFS 탐색 전에 위치 수정
		m_TileCenter->SetTileObject(m_pPlayer->GetGridPos(), nullptr);
		m_TileCenter->SetTileObject(GRID(vDestination), m_pPlayer);

		// 1. 적군 탐색 후, 적군이 있으면 함수 리턴 후 공격상태 들어감(lstTargetEnemy 배열에 적들 정보 들어감)
		// Update에서 적 리스트 빈거 체크하고, 안비었으면 함수 종료 후 공격상태로 돌입
		vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
		list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
		BFSSearch::BFS(GRID(vDestination), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 1);

		// 적군이 있으면, 공격 상태 돌입
		if (!lstMonsters.empty())
		{
			_pScene->ChangeTurn(TURN_TYPE::PLAYER_ATTACK);
		}
		m_TileCenter->InitTileVisited();

		// 2. 타일 상태 갱신 -> 이동한 발판은 검은색 처리
		Vec2 gridDestination = moveRoute.front();
		vector<vector<TileState>>& tiles = m_TileCenter->GetTiles();
		tiles[(int)gridDestination.y][(int)gridDestination.x].pTile->SetTileState(TILE_STATE::BLACK);

		// 3. 타일 상태 갱신(목적지, 현재위치 갱신), 타일 리스트 한칸 삭제		
		m_pPlayer->SetGridPos(moveRoute.front());
		moveRoute.pop_front();
		if (moveRoute.empty()) return;		// 목적지 타일 도착하면, 함수 탈출

		vDestination = REAL(moveRoute.front());
	}

	// 캐릭터에게 도착지쪽으로 움직이게 명령
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), GRID(vDestination), m_pPlayer->GetPos(), vDestination);
	m_pPlayer->Move(gridDirection, vDestination);
}