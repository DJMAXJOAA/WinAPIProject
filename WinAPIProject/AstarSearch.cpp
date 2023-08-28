#include "pch.h"
#include "AstarSearch.h"
#include <queue>
using std::priority_queue;

vector<Node*> AstarSearch::AStar(vector<vector<TileState>>& vecTiles, Vec2 _startPos, Vec2 _endPos, int _move)
{
    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    Node startNode(_startPos);

    startNode.fHeuristic = m_fWeight * Node::Heuristic(_startPos, _endPos);
    openSet.push(new Node(startNode));

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == (int)_endPos.x && current->y == (int)_endPos.y) {
            vector<Node*> path;
            for (Node* node = current; node != nullptr; node = node->pParent) {
                path.push_back(new Node(*node));
            }
            return path;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int x = current->x + dx;
                int y = current->y + dy;

                if (x >= 0 && x < vecTiles.size() && y >= 0 && y < vecTiles[0].size()) {
                    if (vecTiles[x][y].bVisited) continue;

                    float tentativeCost = current->fCost + sqrt(dx * dx + dy * dy);
                    if (tentativeCost > 3) continue; // Limit to 3 steps

                    Node* neighbor = new Node(x, y, tentativeCost, current);
                    Vec2 neighborVec2(x, y);
                    neighbor->fHeuristic = m_fWeight * Node::Heuristic(neighborVec2, _endPos);
                    openSet.push(neighbor);
                }
            }
        }
    }

    return vector<Node*>();
}

vector<vector<Node*>> AstarSearch::SequentialAstar(vector<vector<TileState>>& vecTiles, vector<Vec2> _startPos, vector<Vec2> _endPos, int _move)
{
    vector<vector<Node*>> allPaths;

    // 가정: startPositions.size() == endPositions.size() == n
    for (size_t i = 0; i < _startPos.size(); ++i) {
        Vec2 start = _startPos[i];
        Vec2 end = _endPos[i];

        // 각 플레이어에 대해 A* 알고리즘 실행
        vector<Node*> path = AStar(vecTiles, start, end, _move);

        // 결과를 allPaths에 추가
        allPaths.push_back(path);
    }

    return allPaths;
}