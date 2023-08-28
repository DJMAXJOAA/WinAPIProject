#pragma once
#include "tile.h"
#include "node.h"

class AstarSearch
{
private:
    float           m_fWeight;      // Weighted A* �˰��� -> ����ġ�� �� �� ����
    list<Node*>     m_lstNodes;     // ���� �Ҵ� ��Ų ������ ��Ƶ�

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
    // A* �˰����� �̿��ؼ�, �� ������Ʈ�� ��ã�⸦ ����
    vector<Vec2> AStar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move);
    // ���� ��ü�� A*�� ���������� ����ǰ� (�׷��� ��ġ�� �ʰ� �̵�) �Լ��� ����
    vector<vector<Vec2>> SequentialAstar(vector<vector<TileState>>& vecTiles, vector<pair<Vec2, Vec2>> positions, int _move);
};

