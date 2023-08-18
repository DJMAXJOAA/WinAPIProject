#include "pch.h"
#include "GameInfo.h"

#include "CCore.h"
#include "CPathMgr.h"

// 랜덤
std::mt19937 rng((unsigned int)time(0));

GameInfo::GameInfo(int _key)
	: CData(_key)
    , m_MapInfo{}
    , m_PlayerInfo{}
    , m_fPlaytime{}
{
}

GameInfo::~GameInfo()
{
}

void GameInfo::SaveData()
{
    wchar_t szName[256] = {};

    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = L"ALL\0*.*\0Save\0*.save\0";			// ALL -> 모든 파일이 다보임, 추가 필터 설정 가능 (파일 형식이 All 도는 tile)
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
    strTileFolder += L"save";

    ofn.lpstrInitialDir = strTileFolder.c_str();				// 파일 열때 초기 경로(content 폴더ㅔㅇ 넣는게 좋을듯)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Modal 방식의 Dialog 창 띄우기 -> 모달 방식의 창은 떠있는 동안 다른 윈도우가 작동을 멈춤
    if (GetSaveFileName(&ofn))
    {
        // 확인창을 누르면 반환값이 존재해서, 파일 저장된다
        // szName에 파일경로로 저장 -> 절대경로

        SaveInfo(szName);
    }
}

void GameInfo::LoadData()
{
    // 저장 경로를 지정하고, savetile을 불러오기
    wchar_t szName[256] = {};

    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = L"ALL\0*.*\0Save\0*.save\0";			// ALL -> 모든 파일이 다보임, 추가 필터 설정 가능 (파일 형식이 All 도는 tile)
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
    strTileFolder += L"save";

    ofn.lpstrInitialDir = strTileFolder.c_str();				// 파일 열때 초기 경로(content 폴더ㅔㅇ 넣는게 좋을듯)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Modal 방식의 Dialog 창 띄우기 -> 모달 방식의 창은 떠있는 동안 다른 윈도우가 작동을 멈춤
    if (GetOpenFileName(&ofn))
    {
        wstring strRelativePath = CPathMgr::GetInstance()->GetRelativePath(szName);
        LoadInfo(strRelativePath);
    }
}

void GameInfo::SaveInfo(const wstring& _strRelativePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strRelativePath.c_str(), L"wb");		// 파일 커널 오브젝트, 경로, 쓰기읽기(w -> 쓰기, r -> 읽기, wb, rb -> 바이너리로 쓰기읽기)
    assert(pFile);

    // 맵 저장
    std::ofstream outfile("map.txt");
    for (int i = 0; i < m_MapInfo.vecMap.size(); i++) {
        for (int j = 0; j < m_MapInfo.vecMap[0].size(); j++) {
            fwrite(&m_MapInfo.vecMap[i][j], sizeof(int), 1, pFile);
        }
        outfile << '\n';
    }
    outfile.close();

    fclose(pFile);
}

void GameInfo::LoadInfo(const wstring& _strRelativePath)
{
}

void GameInfo::CreateRandomMap()
{
    // 세이브 데이터가 없을 때(첫 시작), 맵 랜덤 생성 후 저장

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

    std::ofstream outfile("map.txt");
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            outfile << grid[i][j];
        }
        outfile << '\n';
    }
    outfile.close();

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

void GameInfo::SaveMapData()
{

}

void GameInfo::LoadMapData()
{
    vector<vector<int>> grid(HEIGHT * 2 - 1, vector<int>(WIDTH * 2 - 1, 0));

    std::ifstream infile("map.txt");
    if (!infile.is_open()) 
    {
        assert(0);
    }

    int value;
    for (int i = 0; i < HEIGHT * 2 - 1; i++) {
        for (int j = 0; j < WIDTH * 2 - 1; j++) {
            infile >> value;
            grid[i][j] = value;
        }
    }
    infile.close();
}

Node* GameInfo::buildTree(const vector<vector<int>>& grid, int x, int y)
{
    if (x < 0 || x >= WIDTH * 2 - 1 || y < 0 || y >= HEIGHT * 2 - 1) return nullptr;

    Node* node = new Node();
    node->x = x;
    node->y = y;
    node->value = grid[y][x];

    if (x + 1 < WIDTH * 2 - 1) {
        if (y - 1 >= 0 && grid[y - 1][x + 1] == 1) // ↗
            node->children.push_back(buildTree(grid, x + 2, y - 2));

        if (grid[y][x + 1] == 2) // →
            node->children.push_back(buildTree(grid, x + 2, y));

        if (y + 1 < HEIGHT * 2 - 1 && grid[y + 1][x + 1] == 3) // ↘
            node->children.push_back(buildTree(grid, x + 2, y + 2));
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
