#include "pch.h"
#include "PlayerStart.h"

#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTileManager.h"
#include "CTurnManager.h"

#include "CScene_Battle.h"

void PlayerStart::Init(CScene_Battle* _pScene)
{
	CLogicCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();

	// 카메라 캐릭터로 초기화
	CCamera::GetInstance()->SetTarget(nullptr);
	CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

	// 리스트 내에 있는 모든 타일들의 색깔을 원래 색상으로 돌리기
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	m_TileCenter->TileRouteInit(moveRoute);

	// 캐릭터 상태 초기화
	m_pPlayer->SetState(PLAYER_STATE::IDLE);
	m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);
	m_pPlayer->SetAttacking(false);

	// 타일 타겟 지정 잠깐 해제 
	m_TileCenter->SetTileObject(m_pPlayer->GetGridPos(), nullptr);

	// 리스트 초기화
	m_TurnCenter->RouteInit();
	m_TurnCenter->GetTargetList().clear();
	m_TileCenter->TileVisitedInit();

	// 콤보 초기화
	m_TurnCenter->SetCombo(0);

	// 플레이어 애니메이션 설정
	m_pPlayer->SetState(PLAYER_STATE::IDLE);

	printf("CScene_Battle::TurnInit :: 플레이어 턴 시작 초기화\n");
}

void PlayerStart::OnTileSelect(CScene_Battle* _pScene, CObject* _pObj)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetTurnCenter();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	Vec2 vPlayerPos = m_pPlayer->GetGridPos();
	Vec2 selectPos = _pObj->GetGridPos();

	// BFS로 8방향 탐색 (주변 1칸), 오브젝트가 존재하면, 
	BFSSearch::BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

	// BFS 탐색결과, 방문 했음 + 위에 오브젝트 없음
	if (m_TileCenter->IsVisited(selectPos) && m_TileCenter->GetObj(selectPos) == nullptr)
	{
		// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
		CTile* tile = (CTile*)_pObj;

		// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기, 턴 전환
		m_TurnCenter->SetTileColor(tile->GetTileState());
		m_TurnCenter->SetSelectTile(selectPos);
		m_TurnCenter->GetMoveRoute().push_back(selectPos);
		tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

		// 카메라 타일로 지정
		CCamera::GetInstance()->SetLookAt(REAL(selectPos));

		// 턴 변경
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_TILESELECT);

		// 타일 콤보 추가
		m_TurnCenter->SetCombo(1);

		// 디버깅
		DEBUG2(selectPos.x, selectPos.y);
		printf("CScene_Battle::TileSelectTrigger :: 타일 그리기 시작\n");
	}
}

void PlayerStart::Handle(CScene_Battle* _pScene)
{
}
