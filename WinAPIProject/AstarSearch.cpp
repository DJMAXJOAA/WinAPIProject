#include "pch.h"
#include "AstarSearch.h"
#include <queue>
using std::priority_queue;

vector<Vec2> AstarSearch::AStar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move)
{
    Vec2 _startPos = posPair.first;
    Vec2 _endPos = posPair.second;
    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    Node startNode(_startPos);

    startNode.fHeuristic = m_fWeight * Node::Heuristic(_startPos, _endPos);
    openSet.push(new Node(startNode));

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == (int)_endPos.x && current->y == (int)_endPos.y) {
            vector<Vec2> vec2Path;
            for (Node* node = current; node != nullptr; node = node->pParent) {
                vec2Path.push_back(Vec2(node->x, node->y));
                delete node; 
            }
            return vec2Path;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int x = current->x + dx;
                int y = current->y + dy;

                if (x >= 0 && x < vecTiles.size() && y >= 0 && y < vecTiles[0].size()) {
                    if (vecTiles[x][y].bVisited) continue;

                    float tentativeCost = float(current->fCost + sqrt(dx * dx + dy * dy));
                    if (tentativeCost > _move) continue; 

                    Node* neighbor = new Node(x, y, tentativeCost, current);
                    Vec2 neighborVec2(x, y);
                    neighbor->fHeuristic = m_fWeight * Node::Heuristic(neighborVec2, _endPos);
                    openSet.push(neighbor);
                }
            }
        }
    }

    return vector<Vec2>();
}

vector<vector<Vec2>> AstarSearch::SequentialAstar(vector<vector<TileState>>& vecTiles, vector<pair<Vec2, Vec2>> positions, int _move)
{
    vector<vector<Vec2>> allPaths;

    for (const auto& posPair : positions) {
        Vec2 start = posPair.first;
        Vec2 end = posPair.second;

        vector<Vec2> vec2Path = AStar(vecTiles, posPair, _move);

        allPaths.push_back(vec2Path);
    }

    return allPaths;
}