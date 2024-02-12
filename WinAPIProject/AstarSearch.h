#pragma once
#include "tile.h"
#include "node.h"

class AstarSearch
{
private:
    struct CompareNode
    {
        bool operator() (Node* a, Node* b)
        {
            return (a->fCost + a->fHeuristic) > (b->fCost + b->fHeuristic);
        }
    };

private:
    AstarSearch() {}
    ~AstarSearch() {}

public:
    static list<Vec2> Astar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move, float _weight = 1.0f);

private:
    static Node* Search(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, unordered_set<Node*> allNodes, float _weight);
    static bool CheckDiagnoalMove(Node* current);
    static list<Vec2> AddPath(vector<vector<TileState>>& vecTiles, list<Vec2>& lstPath, pair<Vec2, Vec2> posPair, int _move, Node* node);
};

