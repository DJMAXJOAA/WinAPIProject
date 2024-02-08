#pragma once

struct MapNode
{
	int x;
	int y;
	int value;
	vector<MapNode*> children;

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
	static vector<MapNode*> CreateStartPath(const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn);

private:
	static vector<vector<int>> InitMap(int width, int height);
	static void GeneratePath(vector<vector<int>>& map, std::mt19937& rng);
	static void SaveMapFile(const vector<vector<int>>& map, const std::string& filename);
	static int GetValueRandomY();
	static int GetRandomStageSelect();

private:
	static MapNode* CreatePath(int x, int y, const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn);
};

