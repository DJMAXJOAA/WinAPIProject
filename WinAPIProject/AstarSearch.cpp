#include "pch.h"
#include "AstarSearch.h"
#include <queue>
using std::priority_queue;
#include <unordered_set>
using std::unordered_set;

list<Vec2> AstarSearch::Astar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move, float _weight)
{
    Vec2 _startPos = posPair.first;
    Vec2 _endPos = posPair.second;

    CObject* prevObj = vecTiles[(int)_startPos.y][(int)_startPos.x].pObj;
    vecTiles[(int)_startPos.y][(int)_startPos.x].pObj = nullptr;

    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    unordered_set<Node*> allNodes;

    Node* startNode = new Node(_startPos.x, _startPos.y);
    allNodes.insert(startNode);

    startNode->fHeuristic = _weight * Node::Heuristic(_startPos, _endPos);
    openSet.push(startNode);

    list<Vec2> lstPath;
    bool reachedEnd = false;

    while (!openSet.empty())
    {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == _endPos.x && current->y == _endPos.y)
        {
            bool isDiagonalMove = false;
            if (current->pParent != nullptr) // 부모 노드가 있는지 확인
            {
                isDiagonalMove = abs(current->x - current->pParent->x) == 1 && abs(current->y - current->pParent->y) == 1;
            }

            // 대각선 이동이 아니면 목적지 도달로 처리
            if (!isDiagonalMove)
            {
                openSet.push(current->pParent);

                reachedEnd = true;
                break;
            }
            else
            {
                continue;
            }
        }

        vecTiles[current->y][current->x].bVisited = true;

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                int x = current->x + dx;
                int y = current->y + dy;

                if (y >= 0 && y < vecTiles.size() && x >= 0 && x < vecTiles[0].size())
                {
                    if (vecTiles[y][x].bVisited) continue;
                    if (vecTiles[y][x].pObj != nullptr && !(x == _endPos.x && y == _endPos.y)) continue;

                    bool isDiagonal = dx != 0 && dy != 0;
                    float moveCost = isDiagonal ? 1.414f : 1.0f;  // 대각선 비용 증가
                    Node* neighbor = new Node(x, y, current->fCost + moveCost, current);
                    allNodes.insert(neighbor);

                    Vec2 neighborVec2(x, y);
                    neighbor->fHeuristic = _weight * Node::Heuristic(neighborVec2, _endPos);
                    openSet.push(neighbor);
                }
            }
        }
    }

    if (reachedEnd)
    {
        Node* node = openSet.top();
        if (node->x != _endPos.x || node->y != _endPos.y)
            lstPath.emplace_back(node->x, node->y);
        node = node->pParent;
        while (node != nullptr && node->pParent != nullptr)
        {
            if (node->x != _endPos.x || node->y != _endPos.y)
                lstPath.emplace_back(node->x, node->y);
            node = node->pParent;
        }
        lstPath.reverse();
    }

    if (lstPath.size() > _move)
    {
        auto it = lstPath.begin();
        std::advance(it, _move);
        lstPath.erase(it, lstPath.end());
    }

	Vec2 lastPos = lstPath.back();
	while (lastPos == _endPos) 
    {
		lstPath.pop_back();
		lastPos = lstPath.back();
	}
	vecTiles[(int)lastPos.y][(int)lastPos.x].pObj = prevObj;
    lstPath.push_front(_startPos);

    for (Node* node : allNodes)
    {
        delete node;
    }

    return lstPath;
}
