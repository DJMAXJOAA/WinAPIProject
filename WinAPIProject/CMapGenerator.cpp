#include "pch.h"
#include "CMapGenerator.h"

#include "CCore.h"

#include "CBtnUI_Stage.h"

#include<ctime>
static std::random_device rd;
static std::mt19937 rng(rd());
#include<fstream>
using std::ios;
using std::locale;
using std::ifstream;
using std::ofstream;

CMapGenerator::CMapGenerator()
{
}

CMapGenerator::~CMapGenerator()
{
}

void CMapGenerator::Traverse(CBtnUI_Stage* node, map<Vec2, CBtnUI_Stage*>& _mapGridBtn)
{
    // Base condition for DFS
    if (node == nullptr)
        return;

    // 현재 노드의 GridPos를 이용해서 맵에 추가
    _mapGridBtn[node->GetGridPos()] = node;

    for (CBtnUI_Stage* child : node->GetChildren())
    {
        if (child != nullptr)
        {
            // 자식 노드에 해당하는 UI 좌표를 찾는다.
            _mapGridBtn[child->GetGridPos()] = child;

            // 자식 노드를 현재 노드의 자식으로 설정
            vector<CBtnUI_Stage*>& temp = node->GetChildren();
            temp.push_back(child);

            // 재귀 호출
            Traverse(child, _mapGridBtn);
        }
    }
}

vector<CBtnUI_Stage*> CMapGenerator::CreateStartPos(const vector<vector<int>>& _vecMap, map<Vec2, CBtnUI_Stage*>& _mapGridBtn)
{
    vector<CBtnUI_Stage*> result{};
    for (int y = 0; y < HEIGHT * 2 - 1; y++)
    {
        if (_vecMap[y][0] >= 4)
        {
            result.push_back(CreatePath(0, y, _vecMap, _mapGridBtn));
        }
    }
    return result;
}

vector<vector<int>> CMapGenerator::CreateRandomMap()
{
    // 바로 시작 버튼을 눌러서 시작하면 (세이브 데이터가 없으면), 랜덤맵 생성
    vector<vector<int>> grid(HEIGHT * 2 - 1, vector<int>(WIDTH * 2 - 1, 0));

    std::uniform_int_distribution<int> dist(0, HEIGHT - 1);

    // 시작점 6개 생성 (시작 좌표 중복 가능)
    for (int line = 0; line < 6; line++) {
        POINT currentPoint{};
        currentPoint.x = 0;
        int iRandom = dist(rng);
        currentPoint.y = iRandom * 2;

        // 첫 방은 무조건 일반 방(int 4)
        grid[currentPoint.y][currentPoint.x] = 4;

        while (currentPoint.x < (WIDTH - 1) * 2) {
            int deltaY = GetValueRandomY();

            if (currentPoint.y + (deltaY * 2) >= 0 && currentPoint.y + (deltaY * 2) < HEIGHT * 2) {
                switch (deltaY) {
                case -1:
                {
                    if (grid[currentPoint.y - 1][currentPoint.x + 1] == 3) //L'↘'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = 2; // L'→';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y - 1][currentPoint.x + 1] = 1; // L'↗';
                    }
                }
                break;
                case  0:
                {
                    grid[currentPoint.y][currentPoint.x + 1] = 2; // L'→';
                }
                break;
                case  1:
                {
                    if (grid[currentPoint.y + 1][currentPoint.x + 1] == 1) //L'↗'
                    {
                        grid[currentPoint.y][currentPoint.x + 1] = 2; // L'→';
                        deltaY = 0;
                    }
                    else
                    {
                        grid[currentPoint.y + 1][currentPoint.x + 1] = 3; // L'↘';
                    }
                }
                break;
                }
                currentPoint.y += deltaY * 2;
            }
            else
            {
                grid[currentPoint.y][currentPoint.x + 1] = 2; // L'→';
            }

            currentPoint.x += 2;
            grid[currentPoint.y][currentPoint.x] = GetRandomStageSelect();
        }
    }

    /* 확인용 맵 출력 코드 -> 2차원 벡터를 WCHAR형으로 수정하고 확인 */
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

CBtnUI_Stage* CMapGenerator::CreatePath(int x, int y, const vector<vector<int>>& _vecMap, map<Vec2, CBtnUI_Stage*>& _mapGridBtn)
{
    if (x < 0 || x >= WIDTH * 2 - 1 || y < 0 || y >= HEIGHT * 2 - 1) return nullptr;

    CBtnUI_Stage* node = new CBtnUI_Stage();
    node->SetGridPos(Vec2(x, y));
    node->SetValue(_vecMap[y][x]);
    _mapGridBtn[Vec2(x, y)] = node;

    if (x + 1 < WIDTH * 2 - 1) {
        if (y - 1 >= 0 && _vecMap[y - 1][x + 1] == 1) // ↗
            node->GetChildren().push_back(CreatePath(x + 2, y - 2, _vecMap, _mapGridBtn));

        if (_vecMap[y][x + 1] == 2) // →
            node->GetChildren().push_back(CreatePath(x + 2, y, _vecMap, _mapGridBtn));

        if (y + 1 < HEIGHT * 2 - 1 && _vecMap[y + 1][x + 1] == 3) // ↘
            node->GetChildren().push_back(CreatePath(x + 2, y + 2, _vecMap, _mapGridBtn));
    }

    return node;
}

int CMapGenerator::GetValueRandomY()
{
    // 1, 0, -1 중에 랜덤으로 하나 반환
    // 맵이 x 갈때마다 엣지가 위로갈지 중간으로갈지 아래로갈지 정하기
    std::uniform_int_distribution<int> dist(-1, 1);
    return dist(rng);
}

int CMapGenerator::GetRandomStageSelect()
{
    // 방 종류 정하기
    // 15% 엘리트방, 10% 상점, 15% 쉼터, 60% 일반방, 나머지 일반방
    std::uniform_int_distribution<int> dist(0, 99);
    int randomValue = dist(rng);

    if (randomValue < 15) return 5;
    if (randomValue < 25) return 6;
    if (randomValue < 40) return 7;
    return 4;
}
