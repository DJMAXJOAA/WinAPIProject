#include "pch.h"
#include "GameInfo.h"

// 랜덤
std::mt19937 rng((unsigned int)time(0));

GameInfo::GameInfo(int _key)
	: CData(_key)
    , m_PlayerInfo{}
    , m_vecMap{}
    , m_vecStartPos{}
    , m_fPlaytime{}
{
}

GameInfo::~GameInfo()
{
    SafeDeleteVec(m_vecStartPos);
}

void GameInfo::SaveData()
{
    json j;
    j["PlayerInfo"] = m_PlayerInfo.to_json();
    j["MapInfo"] = m_vecMap;
    j["PlayTime"] = m_fPlaytime;

    ofstream outFile("save.json");
    if (!outFile.is_open())
    {
        assert(0);
    }
    outFile << j.dump(4);
    outFile.close();
}

void GameInfo::LoadData()
{
    json j;

    ifstream inFile("save.json");
    if (!inFile.is_open())
    {
        assert(0);
    }
    inFile >> j;
    inFile.close();

    m_PlayerInfo.from_json(j["PlayerInfo"]);
    m_vecMap = j["MapInfo"];
    m_fPlaytime = j["PlayTime"];

    CreateStartPos();
}

void GameInfo::CreateRandomMap()
{
    // 바로 시작 버튼을 눌러서 시작하면 (세이브 데이터가 없으면), 랜덤맵 생성

    srand((unsigned int)time(0));

    vector<vector<int>> grid(HEIGHT * 2 - 1, vector<int>(WIDTH * 2 - 1, 0));

    // 시작점 6개 생성 (시작 좌표 중복 가능)
    for (int line = 0; line < 6; line++) {
        POINT currentPoint;
        currentPoint.x = 0;
        currentPoint.y = (rand() % HEIGHT) * 2;

        // 4, 5, 6, 7중에 하나가 들어감 (방 랜덤 정하기)
        grid[currentPoint.y][currentPoint.x] = getRandomStageSelect();

        while (currentPoint.x < (WIDTH - 1) * 2) {
            int deltaY = getValueRandomY();

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
            grid[currentPoint.y][currentPoint.x] = getRandomStageSelect();
        }
    }

    m_vecMap = grid;

    CreateStartPos();

    /* 확인용 맵 출력 코드 -> 2차원 벡터를 WCHAR형으로 수정하고 확인 */
    //locale::global(locale(".UTF-8"));
    //std::wofstream outfile("map.txt");
    //for (int i = 0; i < grid.size(); i++) {
    //    for (int j = 0; j < grid[0].size(); j++) {
    //        outfile.put(grid[i][j]);
    //    }
    //    outfile.put(L'\n');
    //}
    //outfile.close();
}

void GameInfo::CreateStartPos()
{
    // 첫 시작점 노드을 Node* head로 이어진 길들을 경로 탐색 후 추가
    for (int y = 0; y < HEIGHT * 2 - 1; y++) {
        if (m_vecMap[y][0] >= 4) {
            m_vecStartPos.push_back(buildTree(0, y));

            break;
        }
    }
}

Node* GameInfo::buildTree(int x, int y)
{
    // 재귀로 노드 생성

    if (x < 0 || x >= WIDTH * 2 - 1 || y < 0 || y >= HEIGHT * 2 - 1) return nullptr;

    Node* node = new Node();
    node->x = x;
    node->y = y;
    node->value = m_vecMap[y][x];

    if (x + 1 < WIDTH * 2 - 1) {
        if (y - 1 >= 0 && m_vecMap[y - 1][x + 1] == 1) // ↗
            node->children.push_back(buildTree(x + 2, y - 2));

        if (m_vecMap[y][x + 1] == 2) // →
            node->children.push_back(buildTree(x + 2, y));

        if (y + 1 < HEIGHT * 2 - 1 && m_vecMap[y + 1][x + 1] == 3) // ↘
            node->children.push_back(buildTree(x + 2, y + 2));
    }

    return node;
}

int GameInfo::getValueRandomY()
{
    // 1, 0, -1
    // 맵이 x 갈때마다 엣지가 위로갈지 중간으로갈지 아래로갈지 정하기
    std::uniform_int_distribution<int> dist(-1, 1);
    return dist(rng);
}

int GameInfo::getRandomStageSelect()
{
    // 방 종류 정하기
    // 15% 엘리트방, 10% 상점, 15% 쉼터, 60% 일반방
    std::uniform_int_distribution<int> dist(0, 99);
    int randomValue = dist(rng);

    if (randomValue < 15) return 5;
    if (randomValue < 25) return 6;
    if (randomValue < 40) return 7;
    return 4;
}
