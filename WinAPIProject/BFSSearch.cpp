#include "pch.h"
#include "BFSSearch.h"
#include <queue>

BFSSearch::BFSSearch()
	: m_vecDirection(3)
{
	m_vecDirection[(int)DIRECTION::FOUR_WAY] = { Vec2(-1, 0), Vec2(1, 0), Vec2(0, -1), Vec2(0, 1) };
	m_vecDirection[(int)DIRECTION::DIAGONAL] = { Vec2(-1, -1), Vec2(1, -1), Vec2(-1, 1), Vec2(1, 1) };
	m_vecDirection[(int)DIRECTION::EIGHT_WAY] = { Vec2(-1, 0), Vec2(1, 0), Vec2(0, -1), Vec2(0, 1), Vec2(-1, -1), Vec2(1, -1), Vec2(-1, 1), Vec2(1, 1) };
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

void BFSSearch::BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, list<CObject*>& _lstTarget, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction{};		// Ž�� ����

	// ���� ����
	direction = m_vecDirection[(int)_dir];

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
				// Ÿ�� ���� ������Ʈ�� ������, Ÿ�� ����Ʈ�� �߰�
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
	// ����Ʈ ���� ���� BFS

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
			}
		}
	}
}