#pragma once
#include "tile.h"

class BFSSearch
{
private:
	BFSSearch() {};
	~BFSSearch() {};

public:
	// BFS �˻� ����� ��ȯ��Ŵ
	static void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, list<CObject*>& _lstTarget, DIRECTION _dir, int _depth);
	static void BFS(Vec2 _startPos, vector<vector<TileState>>& _vecTiles, DIRECTION _dir, int _depth);

private:
	static bool isValid(Vec2 _vPos, const vector<vector<TileState>>& _vecTiles);
};
