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

	// ī�޶� ĳ���ͷ� �ʱ�ȭ
	CCamera::GetInstance()->SetTarget(nullptr);
	CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

	// ����Ʈ ���� �ִ� ��� Ÿ�ϵ��� ������ ���� �������� ������
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	m_TileCenter->TileRouteInit(moveRoute);

	// ĳ���� ���� �ʱ�ȭ
	m_pPlayer->SetState(PLAYER_STATE::IDLE);
	m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);
	m_pPlayer->SetAttacking(false);

	// Ÿ�� Ÿ�� ���� ��� ���� 
	m_TileCenter->SetTileObject(m_pPlayer->GetGridPos(), nullptr);

	// ����Ʈ �ʱ�ȭ
	m_TurnCenter->RouteInit();
	m_TurnCenter->GetTargetList().clear();
	m_TileCenter->TileVisitedInit();

	// �޺� �ʱ�ȭ
	m_TurnCenter->SetCombo(0);

	// �÷��̾� �ִϸ��̼� ����
	m_pPlayer->SetState(PLAYER_STATE::IDLE);

	printf("CScene_Battle::TurnInit :: �÷��̾� �� ���� �ʱ�ȭ\n");
}

void PlayerStart::OnTileSelect(CScene_Battle* _pScene, CObject* _pObj)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetTurnCenter();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	Vec2 vPlayerPos = m_pPlayer->GetGridPos();
	Vec2 selectPos = _pObj->GetGridPos();

	// BFS�� 8���� Ž�� (�ֺ� 1ĭ), ������Ʈ�� �����ϸ�, 
	BFSSearch::BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

	// BFS Ž�����, �湮 ���� + ���� ������Ʈ ����
	if (m_TileCenter->IsVisited(selectPos) && m_TileCenter->GetObj(selectPos) == nullptr)
	{
		// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
		CTile* tile = (CTile*)_pObj;

		// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�, �� ��ȯ
		m_TurnCenter->SetTileColor(tile->GetTileState());
		m_TurnCenter->SetSelectTile(selectPos);
		m_TurnCenter->GetMoveRoute().push_back(selectPos);
		tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

		// ī�޶� Ÿ�Ϸ� ����
		CCamera::GetInstance()->SetLookAt(REAL(selectPos));

		// �� ����
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_TILESELECT);

		// Ÿ�� �޺� �߰�
		m_TurnCenter->SetCombo(1);

		// �����
		DEBUG2(selectPos.x, selectPos.y);
		printf("CScene_Battle::TileSelectTrigger :: Ÿ�� �׸��� ����\n");
	}
}

void PlayerStart::Handle(CScene_Battle* _pScene)
{
}
