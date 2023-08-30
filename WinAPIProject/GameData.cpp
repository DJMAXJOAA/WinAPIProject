#include "pch.h"
#include "GameData.h"

#include "CPathMgr.h"

GameData::GameData(int _key)
    : CData(0)
{
}

GameData::~GameData()
{
}

void GameData::SaveData()
{
    // ���̺� �����ʹ� �ϳ��⶧���� ���� �˻������� �ʿ����
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += L"data\\save.json";

    json j;
    j["PlayerInfo"] = m_PlayerInfo;
    j["MapInfo"] = m_vecMap;
    j["MoveRoute"] = vectorVec2_to_json(m_vecRoute);
    j["PlayTime"] = m_fPlaytime;

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
    m_fPlaytime = item["PlayTime"];
}

void to_json(json& j, const PlayerInfo& p)
{
    j = json
    {
    {"Money", p.fMoney},
    {"MaxHP", p.fMaxHP},
    {"CurrentHP", p.fCurHP},
    {"Inventory", vector_wstring_to_json(p.vecInventory)},
    {"Skill", vector_wstring_to_json(p.vecSkill)}
    };
}

void from_json(const json& j, PlayerInfo& p)
{
    p.fMoney = j["Money"].get<float>();
    p.fMaxHP = j["MaxHP"].get<float>();
    p.fCurHP = j["CurrentHP"].get<float>();
    p.vecInventory = json_to_vector_wstring(j["Inventory"].get<vector<string>>());
    p.vecSkill = json_to_vector_wstring(j["Skill"].get<vector<string>>());
}
