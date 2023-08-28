#pragma once
#include "tile.h"

class Node {
public:
    int x, y;
    float       fCost;      // F Value
    float       fHeuristic; // G + H Value를 계산한 휴리스틱
    Node*       pParent;

    Node(int _x, int _y, float cost = 0.f, Node* parent = nullptr)
        : x(_x), y(_y), fCost(cost), fHeuristic(0), pParent(parent) {}
    Node(Vec2 _vPos, float cost = 0.f, Node* parent = nullptr)
        : x((int)_vPos.x), y((int)_vPos.y), fCost(cost), fHeuristic(0), pParent(parent) {}

    static float Heuristic(const Vec2& a, const Vec2& b)
    {
        return a.DistanceTo(b);
    }
};

class AstarSearch
{
private:
    float       m_fWeight;      // Weighted A* 알고리즘 -> 가중치를 둘 수 있음

    struct CompareNode 
    {
        bool operator() (Node* a, Node* b)
        {
            return a->fCost + a->fHeuristic > b->fCost > b->fHeuristic;
        }
    };

public:
    AstarSearch(float _weight = 1.0f) : m_fWeight(_weight) {}
    ~AstarSearch() {}

public:
    vector<Node*> AStar(vector<vector<TileState>>& vecTiles, Vec2 _startPos, Vec2 _endPos, int _move);
    vector<vector<Node*>> SequentialAstar(vector<vector<TileState>>& vecTiles, vector<Vec2> _startPos, vector<Vec2> _endPos, int _move);
};

