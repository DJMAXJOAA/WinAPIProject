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

	// �ߺ��� ��ġ�� ����Ʈ�� ���� ���ϰ� ����
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

			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// Ÿ�� �޺� �߰�
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);

			// �����
			printf("%d �޺� :: ", m_TurnCenter->GetCombo());
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
}

void PlayerTurn_TileSelect::Handle(CScene_Battle* _pScene)
{
}
