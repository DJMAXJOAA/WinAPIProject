#pragma once

struct MapNode
{
	int x;
	int y;
	int value;
	vector<MapNode*> children;  // ����� �ڽĵ� ���� (��ã�� ��Ʈ)

	MapNode() : x(0), y(0), value(0) {};
	~MapNode()
	{
		for (MapNode* child : children)
			delete child;
	}
};

class CMapGenerator
{
public:
	CMapGenerator();
	~CMapGenerator();

public:
	vector<vector<int>> CreateRandomMap();
	vector<MapNode*> CreateStartPos(const vector<vector<int>>& _vecMap);

public:
	MapNode* CreatePath(int x, int y, const vector<vector<int>>& _vecMap);
	int GetValueRandomY();
	int GetRandomStageSelect();
};

