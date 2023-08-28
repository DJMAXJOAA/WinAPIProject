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
	AstarSearch* m_Astar = _pScene->GetAstar();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	pair<Vec2, Vec2> StartToEnd = make_pair(_pMon->GetGridPos(), m_pPlayer->GetGridPos());
	int moveAmount = _pMon->GetMove();
	m_TileCenter->TileVisitedInit();

	// 길찾기 결과
	vector<Vec2> nodeList = m_Astar->AStar(vecTiles, StartToEnd, moveAmount);

	m_TileCenter->TileVisitedInit();
	for (auto& route : nodeList)
	{
		printf("경로 :: x = %1.f, y = %1.f\n", route.x, route.y);
	}

	printf("\n");
}
