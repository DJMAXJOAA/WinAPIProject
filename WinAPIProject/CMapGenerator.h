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

class CBtnUI_Stage;

class CMapGenerator
{
public:
	CMapGenerator();
	~CMapGenerator();

public:
	void Traverse(CBtnUI_Stage* node, map<Vec2, CBtnUI_Stage*>& _mapGridBtn);

public:
	vector<vector<int>> CreateRandomMap();
	vector<CBtnUI_Stage*> CreateStartPos(const vector<vector<int>>& _vecMap, map<Vec2, CBtnUI_Stage*>& _mapGridBtn);

public:
	CBtnUI_Stage* CreatePath(int x, int y, const vector<vector<int>>& _vecMap, map<Vec2, CBtnUI_Stage*>& _mapGridBtn);
	int GetValueRandomY();
	int GetRandomStageSelect();
};

