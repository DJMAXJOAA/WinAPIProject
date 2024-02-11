#include "pch.h"
#include "Monster_RandomMove.h"

#include "CScene_Battle.h"
#include "CMonster.h"
#include "CTileManager.h"

static std::random_device rd;
static std::mt19937 rng(rd());

void Monster_RandomMove::Handle(CScene_Battle* _pScene, CMonster* _pMon)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();
	int moveAmount = _pMon->GetMove();
	m_TileCenter->InitTileVisited();

	list<Vec2> moveList;
	Vec2 currentPos = _pMon->GetGridPos();
	vecTiles[(int)currentPos.y][(int)currentPos.x].pObj = nullptr;

	for (int i = 0; i < moveAmount; i++) {
		currentPos = RandomVec2(vecTiles, currentPos);
		moveList.push_back(currentPos);
	}
	_pMon->SetRoute(moveList);
	vecTiles[(int)currentPos.y][(int)currentPos.x].pObj = _pMon;

	int i = 1;
	for (auto& route : moveList)
	{
		printf("·£´ý ÀÌµ¿ %d¹øÂ°\n", i++);
		printf("°æ·Î :: x = %1.f, y = %1.f\n", route.x, route.y);
	}
	printf("\n");
}

Vec2 Monster_RandomMove::RandomVec2(vector<vector<TileState>>& vecTiles, Vec2 _vPos)
{
	int calX = RandomInteger((int)_vPos.x, BATTLE_SETTINGS::GRID_X);
	int calY = RandomInteger((int)_vPos.y, BATTLE_SETTINGS::GRID_Y);
	if (vecTiles[calY][calX].pObj != nullptr) {
		printf("°ãÄ§");
		return RandomVec2(vecTiles, _vPos);
	}
	return Vec2(calX, calY);
}

int Monster_RandomMove::RandomInteger(int _iVal, int _iRange)
{
	std::uniform_int_distribution<int> dist(-1, 1);

	_iVal += dist(rng);
	if (_iVal >= _iRange) _iVal -= 1;
	else if (_iVal < 0) _iVal += 1;

	return _iVal;
}
