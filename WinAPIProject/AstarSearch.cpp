#include "pch.h"
#include "AstarSearch.h"
#include <queue>
using std::priority_queue;

#include <queue>
#include <vector>
#include <cmath>
#include <unordered_set>
// ... 기타 필요한 헤더

// ... Node, Vec2, TileState, CompareNode의 정의

list<Vec2> AstarSearch::Astar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move) {
    Vec2 _startPos = posPair.first;
    Vec2 _endPos = posPair.second;

    CObject* prevObj = vecTiles[(int)_startPos.y][(int)_startPos.x].pObj;
    vecTiles[(int)_startPos.y][(int)_startPos.x].pObj = nullptr;

    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    std::unordered_set<Node*> allNodes;

    Node* startNode = new Node(_startPos.x, _startPos.y);
    allNodes.insert(startNode);

    startNode->fHeuristic = m_fWeight * Node::Heuristic(_startPos, _endPos);
    openSet.push(startNode);

    list<Vec2> lstPath;  // vec2Path -> lstPath
    bool reachedEnd = false;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == _endPos.x && current->y == _endPos.y) {
            reachedEnd = true;
            break;
        }

        vecTiles[current->y][current->x].bVisited = true;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int x = current->x + dx;
                int y = current->y + dy;

                if (y >= 0 && y < vecTiles.size() && x >= 0 && x < vecTiles[0].size()) {
                    if (vecTiles[y][x].bVisited) continue;
                    if (vecTiles[y][x].pObj != nullptr && !(x == _endPos.x && y == _endPos.y)) continue;

                    Node* neighbor = new Node(x, y, current->fCost + 1.0f, current);
                    allNodes.insert(neighbor);

                    Vec2 neighborVec2(x, y);
                    neighbor->fHeuristic = m_fWeight * Node::Heuristic(neighborVec2, _endPos);
                    openSet.push(neighbor);
                }
            }
        }
    }

    if (reachedEnd) {
        Node* node = openSet.top();
        lstPath.emplace_back(node->x, node->y);  // 목적지 노드 추가
        node = node->pParent;
        while (node != nullptr && node->pParent != nullptr) {
            lstPath.emplace_back(node->x, node->y);
            node = node->pParent;
        }
        lstPath.reverse();  // std::reverse 대신에 list::reverse 사용
    }

    if (lstPath.size() > _move) {
        auto it = lstPath.begin();
        std::advance(it, _move);        // 
        lstPath.erase(it, lstPath.end());
    }

    if (!lstPath.empty()) {  // 빈 리스트인지 확인
        Vec2 lastPos = lstPath.back();  // 마지막 요소를 가져옴
        vecTiles[(int)lastPos.y][(int)lastPos.x].pObj = prevObj;
    }

    for (Node* node : allNodes) {
        delete node;
    }

    return lstPath;
}