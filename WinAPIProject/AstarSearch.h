#pragma once
#include "tile.h"
#include "node.h"

class AstarSearch
{
private:
    float           m_fWeight;      // Weighted A* 알고리즘 -> 가중치를 둘 수 있음
    list<Node*>     m_lstNodes;     // 동적 할당 시킨 노드들을 담아둠

    struct CompareNode
    {
        bool operator() (Node* a, Node* b)
        {
            return (a->fCost + a->fHeuristic) > (b->fCost + b->fHeuristic);
        }
    };

public:
    AstarSearch(float _weight = 1.0f) : m_fWeight(_weight) {}
    ~AstarSearch() {}

public:
    list<Vec2> Astar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move);
};

