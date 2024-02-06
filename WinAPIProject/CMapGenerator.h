#pragma once

struct MapNode
{
	int x;
	int y;
	int value;
	vector<MapNode*> children;  // 노드의 자식들 저장 (길찾기 루트)

	MapNode() : x(0), y(0), value(0) {};
	~MapNode()
	{
		for (MapNode* child : children)
			delete child;
	}
};

class CMapGenerator
{
private:
	CMapGenerator() {}
	~CMapGenerator() {}

public:
	static vector<vector<int>> CreateRandomMap();
	static vector<MapNode*> CreateStartPos(const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn);

private:
	static int GetValueRandomY();
	static int GetRandomStageSelect();
	static MapNode* CreatePath(int x, int y, const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn);
};

