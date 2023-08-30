#include "pch.h"
#include "GameData.h"

#include "CPathMgr.h"

#include "CMapGenerator.h"

GameData::GameData(int _key)
    : CData(0)
{
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += L"data\\save.json";

    ifstream inFile(strFilePath);
    if (!inFile.is_open())
    {
        assert(0);
        return;
    }

    json j;
    inFile >> j;
    inFile.close();

    ParseData(j);
}

GameData::~GameData()
{
}

void GameData::Init()
{
    CMapGenerator mapGenrator;

    // 맵 생성
    m_vecMap = mapGenrator.CreateRandomMap();

    // 플레이어 정보 초기화
    m_PlayerInfo = PlayerInfo
    {
        0.f,
        100.f,
        100.f,
        {},
        {},
    };

    SaveData();
}

void GameData::SaveData()
{
    // 세이브 데이터는 하나기때문에 따로 검색과정이 필요없음
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += L"data\\save.json";

    json j;
    j["PlayerInfo"] = m_PlayerInfo;
    j["MapInfo"] = m_vecMap;
    j["MoveRoute"] = vectorVec2_to_json(m_vecRoute);

    ofstream outFile(strFilePath);
    if (!outFile.is_open())
    {
        assert(0);
    }
    outFile << j.dump(4);
    outFile.close();
}

void GameData::ParseData(const json& item)
{
    m_PlayerInfo = item["PlayerInfo"].get<PlayerInfo>();
    m_vecMap = item["MapInfo"];
    m_vecRoute = json_to_vectorVec2(item["MoveRoute"]);
}

void to_json(json& j, const PlayerInfo& p)
{
    j = json
    {
    {"Money", p.fMoney},
    {"MaxHP", p.fMaxHP},
    {"CurrentHP", p.fCurHP},
    //{"Inventory", vector_wstring_to_json(p.vecInventory)},
    //{"Skill", vector_wstring_to_json(p.vecSkill)}
    };
}

void from_json(const json& j, PlayerInfo& p)
{
    p.fMoney = j["Money"].get<float>();
    p.fMaxHP = j["MaxHP"].get<float>();
    p.fCurHP = j["CurrentHP"].get<float>();
    //p.vecInventory = json_to_vector_wstring(j["Inventory"].get<vector<string>>());
    //p.vecSkill = json_to_vector_wstring(j["Skill"].get<vector<string>>());
}
