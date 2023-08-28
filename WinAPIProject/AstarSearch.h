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
    // A* 알고리즘을 이용해서, 각 오브젝트의 길찾기를 진행
    vector<Vec2> AStar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move);
    // 여러 객체의 A*를 순차적으로 진행되게 (그래야 겹치지 않고 이동) 함수를 설정
    vector<vector<Vec2>> SequentialAstar(vector<vector<TileState>>& vecTiles, vector<pair<Vec2, Vec2>> positions, int _move);
};

