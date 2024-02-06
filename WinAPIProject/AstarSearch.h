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
};

