#pragma once
#include "tile.h"

class CScene_Battle;

class BFSSearch
{
private:
	vector<CObject*>	m_vecTarget;	// BFS Ž�� ����� ����

public:
	BFSSearch();
	~BFSSearch();

public:
	vector<CObject*>& GetTarget() { return m_vecTarget; }

public:
	bool isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles);
	void BFS_Init(vector<vector<TileState>>& _vecTiles);

	// BFS �˻� ����� ��ȯ��Ŵ
	void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth);
};

