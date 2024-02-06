#include "pch.h"
#include "BFSSearch.h"
#include <queue>

const vector<vector<Vec2>> BFS_DIRECTION = 
{
	{ Vec2(-1,  0), Vec2(1,  0), Vec2( 0, -1), Vec2(0, 1) },      // DIRECTION::FOUR_WAY
	{ Vec2(-1, -1), Vec2(1, -1), Vec2(-1,  1), Vec2(1, 1) },    // DIRECTION::DIAGONAL
	{ Vec2(-1,  0), Vec2(1,  0), Vec2( 0, -1), Vec2(0, 1),
	  Vec2(-1, -1), Vec2(1, -1), Vec2(-1,  1), Vec2(1, 1) }     // DIRECTION::EIGHT_WAY
};

bool BFSSearch::isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles)
{
	// 격자 밖으로 나가거나, 방문 했었으면 false
	return (_vPos.x >= 0 && _vPos.x < GRID_X &&
		_vPos.y >= 0 && _vPos.y < GRID_Y &&
		!_vecTiles[(int)_vPos.y][(int)_vPos.x].bVisited);
}

void BFSSearch::BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, list<CObject*>& _lstTarget, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction{};		// 탐색 방향

	// 방향 선택
	direction = BFS_DIRECTION[(int)_dir];

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
				// 타일 위에 오브젝트가 있으면, 타겟 리스트에 추가
				if (_vecTiles[(int)searchPos.y][(int)searchPos.x].pObj != nullptr)
					_lstTarget.push_back(_vecTiles[(int)searchPos.y][(int)searchPos.x].pObj);

				_vecTiles[(int)searchPos.y][(int)searchPos.x].bVisited = true;

				q.push(searchPos);
				moveCount.push(count + 1);
			}
		}
	}
}

void BFSSearch::BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth)
{
	// 리스트 없는 버전 BFS

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
			}
		}
	}
}