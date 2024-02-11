#include "pch.h"
#include "PlayerTurn_TileSelect.h"

#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTileManager.h"
#include "CTurnManager.h"

#include "CScene_Battle.h"

#include "CKeyMgr.h"

void PlayerTurn_TileSelect::OnTileSelect(CScene_Battle* _pScene, CObject* _pObj)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	Vec2 selectPos = _pObj->GetGridPos();
	Vec2 currentPos = m_TurnCenter->GetSelectTile();

	BFSSearch::BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

	// 중복된 위치는 리스트에 들어가지 못하게 설정
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);

	if (m_TileCenter->IsVisited(selectPos) &&
		m_TileCenter->GetObj(selectPos) == nullptr &&
		iter == moveRoute.end())
	{
		if (m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor())
		{   // 첫 타일과 색갈이 다르다면, 새로운 루트로 지정할 수 없음
			CTile* tile = (CTile*)_pObj;

			// 카메라 선택한 타일로 지정
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 타일 콤보 추가
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);
		}
	}
}

void PlayerTurn_TileSelect::Handle(CScene_Battle* _pScene)
{
	// 타일 결정
	if (KEY_TAP(KEY::CTRL))
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
	}
	// 선택 취소
	if (KEY_TAP(KEY::ESC))
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_START);
	}
}
