#include "pch.h"
#include "AstarSearch.h"

list<Vec2> AstarSearch::Astar(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, int _move, float _weight)
{
    unordered_set<Node*> allNodes;  // Node* delete
    list<Vec2> lstPath;             // ��ȯ�� ��Ʈ ����Ʈ

    Vec2 startPos = posPair.first;
    Vec2 endPos = posPair.second;

    // �̵� �� Ÿ�� ���� ����
    CObject* prevObj = vecTiles[(int)startPos.y][(int)startPos.x].pObj;
    vecTiles[(int)startPos.y][(int)startPos.x].pObj = nullptr;

    // A* Ž��
    Node* node = Search(vecTiles, posPair, allNodes, _weight);
    if (node != nullptr) 
    {   
        // ��ȯ�� ����Ʈ�� �̵� ��Ʈ�� �߰�
        AddPath(vecTiles, lstPath, posPair, _move, node);
        
        // �̵� �� Ÿ�� ���� ����
        Vec2 lastPos = lstPath.back();
        vecTiles[(int)lastPos.y][(int)lastPos.x].pObj = prevObj;
        lstPath.push_front(startPos);
    }
    else
    {   
        // �̵� �Ұ� :: Ÿ�� ���� ����
        vecTiles[(int)startPos.y][(int)startPos.x].pObj = prevObj;
    }

    for (Node* node : allNodes)
    {
        delete node;
    }
    return lstPath;
}

Node* AstarSearch::Search(vector<vector<TileState>>& vecTiles, pair<Vec2, Vec2> posPair, unordered_set<Node*> allNodes, float _weight)
{
    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    Vec2 startPos = posPair.first; Vec2 endPos = posPair.second;

    Node* startNode = new Node(startPos.x, startPos.y);
    startNode->fHeuristic = _weight * Node::Heuristic(startPos, endPos);
    allNodes.insert(startNode);
    openSet.push(startNode);

    while (!openSet.empty())
    {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == endPos.x && current->y == endPos.y)
        {
            // ���� �̵��� �밢�� �̵��̸� ó�� X
            if (!CheckDiagnoalMove(current))
            {
                return current->pParent;
            }
            continue;
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
                    if (vecTiles[y][x].pObj != nullptr && !(x == endPos.x && y == endPos.y)) continue;

                    bool isDiagonal = dx != 0 && dy != 0;
                    float moveCost = isDiagonal ? 1.414f : 1.0f;  // �밢�� ��� ���� (sqrt(2))
                    Node* neighbor = new Node(x, y, current->fCost + moveCost, current);
                    allNodes.insert(neighbor);

                    Vec2 neighborVec2(x, y);
                    neighbor->fHeuristic = _weight * Node::Heuristic(neighborVec2, endPos);
                    openSet.push(neighbor);
                }
            }
        }
    }
    return nullptr;
}

bool AstarSearch::CheckDiagnoalMove(Node* current)
{
    if (current->pParent != nullptr) // �θ� ��尡 �ִ��� Ȯ��
    {   // ���� ���� �밢�� �̵� Ȯ��
        return abs(current->x - current->pParent->x) == 1 && abs(current->y - current->pParent->y) == 1;
    }
    return false;
}

list<Vec2> AstarSearch::AddPath(vector<vector<TileState>>& vecTiles, list<Vec2>& lstPath, pair<Vec2, Vec2> posPair, int _move, Node* node)
{
    Vec2 startPos = posPair.first; Vec2 endPos = posPair.second;

    // ��� �ְ� ��� ������
    lstPath.emplace_back(node->x, node->y);
    node = node->pParent;
    while (node != nullptr && node->pParent != nullptr)
    {
        lstPath.emplace_back(node->x, node->y);
        node = node->pParent;
    }
    lstPath.reverse();

    // �̵�����ŭ �߶󳻱�
    if (lstPath.size() > _move)
    {
        auto it = lstPath.begin();
        std::advance(it, _move);
        lstPath.erase(it, lstPath.end());
    }

    // ���������� ��ĥ��� (�÷��̾�) ���� ����ó��
    Vec2 lastPos = lstPath.back();
    while (lastPos == endPos)
    {
        lstPath.pop_back();
        lastPos = lstPath.back();
    }

    return lstPath;
}
