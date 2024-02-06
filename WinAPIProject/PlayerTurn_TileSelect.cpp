#include "pch.h"
#include "PlayerTurn_TileSelect.h"

#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTileManager.h"
#include "CTurnManager.h"

#include "CScene_Battle.h"

void PlayerTurn_TileSelect::OnTileSelect(CScene_Battle* _pScene, CObject* _pObj)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	Vec2 selectPos = _pObj->GetGridPos();
	Vec2 currentPos = m_TurnCenter->GetSelectTile();
	BFSSearch::BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

	// 중복된 위치는 리스트에 들어가지 못하게 설정
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);

	if (m_TileCenter->IsVisited(selectPos) &&
		/*m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor() &&*/
		m_TileCenter->GetObj(selectPos) == nullptr &&
		iter == moveRoute.end())
	{
		if (m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor())
		{
			CTile* tile = (CTile*)_pObj;

			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 타일 콤보 추가
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);

			// 디버깅
			printf("%d 콤보 :: ", m_TurnCenter->GetCombo());
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
}

void PlayerTurn_TileSelect::Handle(CScene_Battle* _pScene)
{
}
