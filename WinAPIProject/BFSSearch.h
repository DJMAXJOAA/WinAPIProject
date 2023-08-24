#pragma once
#include "tile.h"

class CScene_Battle;
using namespace battle;

class BFSSearch
{
private:
	vector<CObject*>	m_vecTarget;	// BFS 탐색 결과를 저장

public:
	BFSSearch(CScene_Battle* _pScene);
	~BFSSearch();

public:
	vector<CObject*>& GetTarget() { return m_vecTarget; }

public:
	bool isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles);
	void BFS_Init(vector<vector<TileState>>& _vecTiles);

	// BFS 검색 결과를 반환시킴
	void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth);
};

