#include "pch.h"
#include "CMapGenerator.h"

static std::random_device rd;
static std::mt19937 rng(rd());
#include<fstream>
using std::ios;
using std::locale;
using std::ifstream;
using std::ofstream;

vector<MapNode*> CMapGenerator::CreateStartPath(const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn)
{
    vector<MapNode*> result{};
    for (int y = 0; y < ROBBY_SETTINGS::HEIGHT * 2 - 1; y++)
    {   // �� ó�� �Ϲ� �濡������ DFS�� �ڽ� ������ �����Ͽ� ��Ʈ�� �ϼ���ŵ�ϴ�.
        if (_vecMap[y][0] >= (int)ROOM_TYPE::NORMAL_ROOM)
        {
            result.push_back(CreatePath(0, y, _vecMap, _mapGridBtn));
        }
    }
    return result;
}

vector<vector<int>> CMapGenerator::CreateRandomMap()
{
    // �ٷ� ���� ��ư�� ������ �����ϸ� (���̺� �����Ͱ� ������), ������ ����
    vector<vector<int>> grid(ROBBY_SETTINGS::HEIGHT * 2 - 1, vector<int>(ROBBY_SETTINGS::WIDTH * 2 - 1, 0));
    std::uniform_int_distribution<int> dist(0, ROBBY_SETTINGS::HEIGHT - 1);

    // ������ 6�� ���� (���� ��ǥ �ߺ� ����)
    for (int line = 0; line < ROBBY_SETTINGS::NUMBER_OF_ROUTE; line++)
    {
        POINT currentPoint{};
        currentPoint.x = 0;
        currentPoint.y = dist(rng) * 2; // ���� ��ġ

        // ù ���� ������ �Ϲ� ��(int 4)
        grid[currentPoint.y][currentPoint.x] = (int)ROOM_TYPE::NORMAL_ROOM;

        while (currentPoint.x < (ROBBY_SETTINGS::WIDTH - 1) * 2)
        {
            int deltaY = GetValueRandomY();

            if (currentPoint.y + (deltaY * 2) >= 0 && currentPoint.y + (deltaY * 2) < ROBBY_SETTINGS::HEIGHT * 2) {
                switch (deltaY) 
                {
                case -1:
                {
                    if (grid[currentPoint.y - 1][currentPoint.x + 1] == (int)ROOM_TYPE::DOWN_DIRECTION) //L'��'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = (int)ROOM_TYPE::RIGHT_DIRECTION; // L'��';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y - 1][currentPoint.x + 1] = (int)ROOM_TYPE::UP_DIRECTION; // L'��';
                    }
                }
                break;
                case  0:
                {
                    grid[currentPoint.y][currentPoint.x + 1] = (int)ROOM_TYPE::RIGHT_DIRECTION; // L'��';
                }
                break;
                case  1:
                {
                    if (grid[currentPoint.y + 1][currentPoint.x + 1] == (int)ROOM_TYPE::UP_DIRECTION) //L'��'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = (int)ROOM_TYPE::RIGHT_DIRECTION; // L'��';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y + 1][currentPoint.x + 1] = (int)ROOM_TYPE::DOWN_DIRECTION; // L'��';
                    }
                }
                break;
                }
                currentPoint.y += deltaY * 2;
            }
            else
            {
                grid[currentPoint.y][currentPoint.x + 1] = (int)ROOM_TYPE::RIGHT_DIRECTION; // L'��';
            }

            currentPoint.x += 2;
            grid[currentPoint.y][currentPoint.x] = GetRandomStageSelect();
        }
    }

    /* Ȯ�ο� �� ��� �ڵ� -> 2���� ���͸� WCHAR������ �����ϰ� Ȯ�� */
    ofstream outfile("map.txt");
    for (int i = 0; i < grid.size(); i++) 
    {
        for (int j = 0; j < grid[0].size(); j++) 
        {
            outfile << grid[i][j];
        }
        outfile << '\n';
    }
    outfile.close();

    return grid;
}

MapNode* CMapGenerator::CreatePath(int x, int y, const vector<vector<int>>& _vecMap, map<Vec2, MapNode*>& _mapGridBtn)
{
    if (x < 0 || x >= ROBBY_SETTINGS::WIDTH * 2 - 1 || y < 0 || y >= ROBBY_SETTINGS::HEIGHT * 2 - 1) return nullptr;

    MapNode* node = new MapNode;
    node->x = x;
    node->y = y;
    node->value = _vecMap[y][x];
    _mapGridBtn[Vec2(x, y)] = node;

    if (x + 1 < ROBBY_SETTINGS::WIDTH * 2 - 1)
    {
        if (y - 1 >= 0 && _vecMap[y - 1][x + 1] == (int)ROOM_TYPE::UP_DIRECTION) 
            node->children.push_back(CreatePath(x + 2, y - 2, _vecMap, _mapGridBtn));   // ��

        if (_vecMap[y][x + 1] == (int)ROOM_TYPE::RIGHT_DIRECTION) 
            node->children.push_back(CreatePath(x + 2, y, _vecMap, _mapGridBtn));       // ��

        if (y + 1 < ROBBY_SETTINGS::HEIGHT * 2 - 1 
            && _vecMap[y + 1][x + 1] == (int)ROOM_TYPE::DOWN_DIRECTION) 
            node->children.push_back(CreatePath(x + 2, y + 2, _vecMap, _mapGridBtn));   // ��
    }

    return node;
}

int CMapGenerator::GetValueRandomY()
{
    // 1, 0, -1 �߿� �������� �ϳ� ��ȯ
    // ���� x �������� ������ ���ΰ��� �߰����ΰ��� �Ʒ��ΰ��� ���ϱ�
    std::uniform_int_distribution<int> dist(-1, 1);
    return dist(rng);
}

int CMapGenerator::GetRandomStageSelect()
{
    // �� ���� ���ϱ� ����
    std::uniform_int_distribution<int> dist(0, 99);
    int randomValue = dist(rng);

    if (randomValue < ROBBY_SETTINGS::ELITE_ROOM_CHANCE) return (int)ROOM_TYPE::ELITE_ROOM;
    if (randomValue < ROBBY_SETTINGS::SHOP_ROOM_CHANCE) return (int)ROOM_TYPE::SHOP_ROOM;
    if (randomValue < ROBBY_SETTINGS::REST_ROOM_CHANCE) return (int)ROOM_TYPE::REST_ROOM;
    return (int)ROOM_TYPE::NORMAL_ROOM;
}
