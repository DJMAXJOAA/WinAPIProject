#include "pch.h"
#include "Monster_Trace.h"

#include "CScene_Battle.h"
#include "CMonster.h"
#include "CPlayer.h"

#include "AstarSearch.h"
#include "CTileManager.h"

void Monster_Trace::Handle(CScene_Battle* _pScene, CMonster* _pMon)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	pair<Vec2, Vec2> StartToEnd = make_pair(_pMon->GetGridPos(), m_pPlayer->GetGridPos());
	int moveAmount = _pMon->GetMove();
	m_TileCenter->TileVisitedInit();

	// ��ã�� ���
	list<Vec2> nodeList = AstarSearch::Astar(vecTiles, StartToEnd, moveAmount);
	_pMon->SetRoute(nodeList);	// ��Ʈ ����

	for (auto& route : nodeList)
	{
		printf("���� ��ǥ : %1.f, %1.f // ���� ��ǥ : %1.f, %1.f // ", _pMon->GetGridPos().x, _pMon->GetGridPos().y, m_pPlayer->GetGridPos().x, m_pPlayer->GetGridPos().y);
		printf("��� :: x = %1.f, y = %1.f\n", route.x, route.y);
	}
	printf("\n");


}
