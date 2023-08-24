#include "pch.h"
#include "BFSSearch.h"
#include <queue>

BFSSearch::BFSSearch()
	: m_vecTarget{}
{
}

BFSSearch::~BFSSearch()
{
}

bool BFSSearch::isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles)
{
	// 격자 밖으로 나가거나, 방문 했었으면 false
	return (_vPos.x >= 0 && _vPos.x < GRID_X &&
		_vPos.y >= 0 && _vPos.y < GRID_Y &&
		!_vecTiles[(int)_vPos.y][(int)_vPos.x].bVisited);
}

void BFSSearch::BFS_Init(vector<vector<TileState>>& _vecTiles)
{
	// BFS의 방문 초기화
	for (size_t i = 0; i < _vecTiles.size(); i++)
	{
		for (size_t j = 0; j < _vecTiles[i].size(); j++)
		{
			_vecTiles[j][i].bVisited = false;
		}
	}
}

void BFSSearch::BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction{};		// 탐색 방향

	// 방향 선택
	switch (_dir)
	{
	case DIRECTION::FOUR_WAY:
		direction.push_back(Vec2(-1, 0));
		direction.push_back(Vec2(1, 0));
		direction.push_back(Vec2(0, -1));
		direction.push_back(Vec2(0, 1));
		break;
	case DIRECTION::DIAGONAL:
		direction.push_back(Vec2(-1, -1));
		direction.push_back(Vec2(1, -1));
		direction.push_back(Vec2(-1, 1));
		direction.push_back(Vec2(1, 1));
		break;
	case DIRECTION::EIGHT_WAY:
		direction.push_back(Vec2(-1, 0));
		direction.push_back(Vec2(1, 0));
		direction.push_back(Vec2(0, -1));
		direction.push_back(Vec2(0, 1));
		direction.push_back(Vec2(-1, -1));
		direction.push_back(Vec2(1, -1));
		direction.push_back(Vec2(-1, 1));
		direction.push_back(Vec2(1, 1));
		break;
	}

	std::queue<Vec2> q;
	std::queue<int> moveCount;	// 각각의 타일마다 탐색 횟수 (깊이 설정)
	q.push(_startPos);
	moveCount.push(0);

	while (!q.empty())
	{
		Vec2 currentPos = q.front();
		int count = moveCount.front();
		q.pop();
		moveCount.pop();

		if (count >= _depth) continue;	// 깊이가 최대깊이 도달하면 스킵

		for (int i = 0; i < direction.size(); i++)
		{
			Vec2 searchPos(currentPos.x + direction[i].x, currentPos.y + direction[i].y);
			if (isValid(searchPos, _vecTiles))
			{
				_vecTiles[(int)searchPos.y][(int)searchPos.x].bVisited = true;
				q.push(searchPos);
				moveCount.push(count + 1);
				break;
			}
		}
	}
}

//switch (m_CurrentTurn)
//{
//case TURN_TYPE::PLAYER_START:
//{
//	// 블럭 선택중일때는, 첫 선택한 블럭의 색깔과 일치해야함
//	m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
//	q.push(searchPos);
//	moveCount.push(count + 1);
//}
//break;
//case TURN_TYPE::PLAYER_TILESELECT:
//{
//	// 블럭 첫선택일때는 그냥 넣음
//	int tile_number = int(searchPos.x + searchPos.y * GRID_Y);
//
//	if (((CTile*)groupTile[tile_number])->GetTileState() == m_TileColor)
//	{
//		m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
//		q.push(searchPos);
//		moveCount.push(count + 1);
//	}
//}
//break;
//case TURN_TYPE::PLAYER_MOVE:
//{
//	// 타일 위에 적군이 있는지 체크
//	int tile_number = int(searchPos.x + searchPos.y * GRID_Y);
//	CMonster* pEnemy = (CMonster*)m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].ObjOnTile;
//
//	if (pEnemy)
//	{
//		// 리스트에 적군들을 넣고, BFS가 끝나면 한번에 처리(리스트 순서대로)
//		m_lstTargetEnemy.push_back(pEnemy);
//
//		m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
//		q.push(searchPos);
//		moveCount.push(count + 1);
//	}
//}