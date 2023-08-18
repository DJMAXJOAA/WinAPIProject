#include "pch.h"
#include "GameInfo.h"

#include "CCore.h"
#include "CPathMgr.h"

// ����
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
    ofn.lpstrFilter = L"ALL\0*.*\0Save\0*.save\0";			// ALL -> ��� ������ �ٺ���, �߰� ���� ���� ���� (���� ������ All ���� tile)
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
    strTileFolder += L"save";

    ofn.lpstrInitialDir = strTileFolder.c_str();				// ���� ���� �ʱ� ���(content �����Ĥ� �ִ°� ������)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Modal ����� Dialog â ���� -> ��� ����� â�� ���ִ� ���� �ٸ� �����찡 �۵��� ����
    if (GetSaveFileName(&ofn))
    {
        // Ȯ��â�� ������ ��ȯ���� �����ؼ�, ���� ����ȴ�
        // szName�� ���ϰ�η� ���� -> ������

        SaveInfo(szName);
    }
}

void GameInfo::LoadData()
{
    // ���� ��θ� �����ϰ�, savetile�� �ҷ�����
    wchar_t szName[256] = {};

    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = L"ALL\0*.*\0Save\0*.save\0";			// ALL -> ��� ������ �ٺ���, �߰� ���� ���� ���� (���� ������ All ���� tile)
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
    strTileFolder += L"save";

    ofn.lpstrInitialDir = strTileFolder.c_str();				// ���� ���� �ʱ� ���(content �����Ĥ� �ִ°� ������)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Modal ����� Dialog â ���� -> ��� ����� â�� ���ִ� ���� �ٸ� �����찡 �۵��� ����
    if (GetOpenFileName(&ofn))
    {
        wstring strRelativePath = CPathMgr::GetInstance()->GetRelativePath(szName);
        LoadInfo(strRelativePath);
    }
}

void GameInfo::SaveInfo(const wstring& _strRelativePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strRelativePath.c_str(), L"wb");		// ���� Ŀ�� ������Ʈ, ���, �����б�(w -> ����, r -> �б�, wb, rb -> ���̳ʸ��� �����б�)
    assert(pFile);

    // �� ����
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
    // ���̺� �����Ͱ� ���� ��(ù ����), �� ���� ���� �� ����

    srand((unsigned int)time(0));

    vector<vector<int>> grid(HEIGHT * 2 - 1, vector<int>(WIDTH * 2 - 1, 0));

    // ������ 6�� ���� (���� ��ǥ �ߺ� ����)
    for (int line = 0; line < 6; line++) {
        POINT currentPoint;
        currentPoint.x = 0;
        currentPoint.y = (rand() % HEIGHT) * 2;

        // 4, 5, 6, 7�߿� �ϳ��� �� (�� ���� ���ϱ�)
        grid[currentPoint.y][currentPoint.x] = getRandomStageSelect();

        while (currentPoint.x < (WIDTH - 1) * 2) {
            int deltaY = getValueRandomY();

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

    /* Ȯ�ο� �� ��� �ڵ� -> 2���� ���͸� WCHAR������ �����ϰ� Ȯ�� */
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
        if (y - 1 >= 0 && grid[y - 1][x + 1] == 1) // ��
            node->children.push_back(buildTree(grid, x + 2, y - 2));

        if (grid[y][x + 1] == 2) // ��
            node->children.push_back(buildTree(grid, x + 2, y));

        if (y + 1 < HEIGHT * 2 - 1 && grid[y + 1][x + 1] == 3) // ��
            node->children.push_back(buildTree(grid, x + 2, y + 2));
    }

    return node;
}

int GameInfo::getValueRandomY()
{
    // 1, 0, -1
    // ���� x �������� ������ ���ΰ��� �߰����ΰ��� �Ʒ��ΰ��� ���ϱ�
    std::uniform_int_distribution<int> dist(-1, 1);
    return dist(rng);
}

int GameInfo::getRandomStageSelect()
{
    // �� ���� ���ϱ�
    // 15% ����Ʈ��, 10% ����, 15% ����, 60% �Ϲݹ�
    std::uniform_int_distribution<int> dist(0, 99);
    int randomValue = dist(rng);

    if (randomValue < 15) return 5;
    if (randomValue < 25) return 6;
    if (randomValue < 40) return 7;
    return 4;
}
