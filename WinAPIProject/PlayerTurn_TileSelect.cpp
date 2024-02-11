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

	// �ߺ��� ��ġ�� ����Ʈ�� ���� ���ϰ� ����
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);

	if (m_TileCenter->IsVisited(selectPos) &&
		m_TileCenter->GetObj(selectPos) == nullptr &&
		iter == moveRoute.end())
	{
		if (m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor())
		{   // ù Ÿ�ϰ� ������ �ٸ��ٸ�, ���ο� ��Ʈ�� ������ �� ����
			CTile* tile = (CTile*)_pObj;

			// ī�޶� ������ Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// Ÿ�� �޺� �߰�
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);
		}
	}
}

void PlayerTurn_TileSelect::Handle(CScene_Battle* _pScene)
{
	// Ÿ�� ����
	if (KEY_TAP(KEY::CTRL))
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
	}
	// ���� ���
	if (KEY_TAP(KEY::ESC))
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_START);
	}
}
