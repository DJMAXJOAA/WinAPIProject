#include "pch.h"
#include "CMapGenerator.h"

#include<ctime>
static std::random_device rd;
static std::mt19937 rng(rd());
#include<fstream>
using std::locale;
using std::ifstream;
using std::ofstream;

CMapGenerator::CMapGenerator()
{
}

CMapGenerator::~CMapGenerator()
{
}

vector<Node*> CMapGenerator::CreateStartPos(const vector<vector<int>>& _vecMap)
{
    vector<Node*> result{};
    // ù ������ ����� Node* head�� �̾��� ����� ��� Ž�� �� �߰�
    for (int y = 0; y < HEIGHT * 2 - 1; y++) {
        if (_vecMap[y][0] >= 4) {
            result.push_back(CreatePath(0, y, _vecMap));

            break;
        }
    }

    return result;
}

vector<vector<int>> CMapGenerator::CreateRandomMap()
{
    // �ٷ� ���� ��ư�� ������ �����ϸ� (���̺� �����Ͱ� ������), ������ ����
    vector<vector<int>> grid(HEIGHT * 2 - 1, vector<int>(WIDTH * 2 - 1, 0));

    std::uniform_int_distribution<int> dist(0, HEIGHT - 1);

    // ������ 6�� ���� (���� ��ǥ �ߺ� ����)
    for (int line = 0; line < 6; line++) {
        POINT currentPoint{};
        currentPoint.x = 0;
        int iRandom = dist(rng);
        currentPoint.y = iRandom * 2;

        // ù ���� ������ �Ϲ� ��(int 4)
        grid[currentPoint.y][currentPoint.x] = 4;

        while (currentPoint.x < (WIDTH - 1) * 2) {
            int deltaY = GetValueRandomY();

            if (currentPoint.y + (deltaY * 2) >= 0 && currentPoint.y + (deltaY * 2) < HEIGHT * 2) {
                switch (deltaY) {
                case -1:
                {
                    if (grid[currentPoint.y - 1][currentPoint.x + 1] == 3) //L'��'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = 2; // L'��';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y - 1][currentPoint.x + 1] = 1; // L'��';
                    }
                }
                break;
                case  0:
                {
                    grid[currentPoint.y][currentPoint.x + 1] = 2; // L'��';
                }
                break;
                case  1:
                {
                    if (grid[currentPoint.y + 1][currentPoint.x + 1] == 1) //L'��'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = 2; // L'��';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y + 1][currentPoint.x + 1] = 3; // L'��';
                    }
                }
                break;
                }
                currentPoint.y += deltaY * 2;
            }
            else
            {
                grid[currentPoint.y][currentPoint.x + 1] = 2; // L'��';
            }

            currentPoint.x += 2;
        }
    }

    /* Ȯ�ο� �� ��� �ڵ� -> 2���� ���͸� WCHAR������ �����ϰ� Ȯ�� */
    ofstream outfile("map.txt");
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            outfile << grid[i][j];
        }
        outfile << '\n';
    }
    outfile.close();

    return grid;
}

Node* CMapGenerator::CreatePath(int x, int y, const vector<vector<int>>& _vecMap)
{
    // ��ͷ� ��� ����

    if (x < 0 || x >= WIDTH * 2 - 1 || y < 0 || y >= HEIGHT * 2 - 1) return nullptr;

    Node* node = new Node();
    node->x = x;
    node->y = y;
    node->value = _vecMap[y][x];

    if (x + 1 < WIDTH * 2 - 1) {
        if (y - 1 >= 0 && _vecMap[y - 1][x + 1] == 1) // ��
            node->children.push_back(CreatePath(x + 2, y - 2, _vecMap));

        if (_vecMap[y][x + 1] == 2) // ��
            node->children.push_back(CreatePath(x + 2, y, _vecMap));

        if (y + 1 < HEIGHT * 2 - 1 && _vecMap[y + 1][x + 1] == 3) // ��
            node->children.push_back(CreatePath(x + 2, y + 2, _vecMap));
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
    // �� ���� ���ϱ�
    // 15% ����Ʈ��, 10% ����, 15% ����, 60% �Ϲݹ�, ������ �Ϲݹ�
    std::uniform_int_distribution<int> dist(0, 99);
    int randomValue = dist(rng);

    if (randomValue < 15) return 5;
    if (randomValue < 25) return 6;
    if (randomValue < 40) return 7;
    return 4;
}
