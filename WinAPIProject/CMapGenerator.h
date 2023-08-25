#pragma once

struct Node
{
	int x;
	int y;
	int value;
	vector<Node*> children;  // 노드의 자식들 저장 (길찾기 루트)

	Node() : x(0), y(0), value(0) {};
	~Node()
	{
		for (Node* child : children)
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
	vector<Node*> CreateStartPos(const vector<vector<int>>& _vecMap);

public:
	Node* CreatePath(int x, int y, const vector<vector<int>>& _vecMap);
	int GetValueRandomY();
	int GetRandomStageSelect();
};

