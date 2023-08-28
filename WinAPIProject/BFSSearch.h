#pragma once
#include "tile.h"

class BFSSearch
{
private:
	vector<vector<Vec2>>	m_vecDirection;

public:
	BFSSearch();
	~BFSSearch();

public:
	bool isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles);

	// BFS �˻� ����� ��ȯ��Ŵ
	void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, list<CObject*>& _lstTarget, DIRECTION _dir, int _depth);
	void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth);
};

