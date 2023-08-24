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
	// ���� ������ �����ų�, �湮 �߾����� false
	return (_vPos.x >= 0 && _vPos.x < GRID_X &&
		_vPos.y >= 0 && _vPos.y < GRID_Y &&
		!_vecTiles[(int)_vPos.y][(int)_vPos.x].bVisited);
}

void BFSSearch::BFS_Init(vector<vector<TileState>>& _vecTiles)
{
	// BFS�� �湮 �ʱ�ȭ
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
	vector<Vec2> direction{};		// Ž�� ����

	// ���� ����
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
	std::queue<int> moveCount;	// ������ Ÿ�ϸ��� Ž�� Ƚ�� (���� ����)
	q.push(_startPos);
	moveCount.push(0);

	while (!q.empty())
	{
		Vec2 currentPos = q.front();
		int count = moveCount.front();
		q.pop();
		moveCount.pop();

		if (count >= _depth) continue;	// ���̰� �ִ���� �����ϸ� ��ŵ

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
//	// �� �������϶���, ù ������ ���� ����� ��ġ�ؾ���
//	m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
//	q.push(searchPos);
//	moveCount.push(count + 1);
//}
//break;
//case TURN_TYPE::PLAYER_TILESELECT:
//{
//	// �� ù�����϶��� �׳� ����
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
//	// Ÿ�� ���� ������ �ִ��� üũ
//	int tile_number = int(searchPos.x + searchPos.y * GRID_Y);
//	CMonster* pEnemy = (CMonster*)m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].ObjOnTile;
//
//	if (pEnemy)
//	{
//		// ����Ʈ�� �������� �ְ�, BFS�� ������ �ѹ��� ó��(����Ʈ �������)
//		m_lstTargetEnemy.push_back(pEnemy);
//
//		m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
//		q.push(searchPos);
//		moveCount.push(count + 1);
//	}
//}