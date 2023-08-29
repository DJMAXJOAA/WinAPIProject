#include "pch.h"
#include "MonsterData.h"

MonsterData::MonsterData(int _key)
	: CData(_key)
{
	LoadData(L"data\\500.json");
}

MonsterData::~MonsterData()
{

}

void MonsterData::ParseData(const json& item)
{
    m_strName = string_to_wstring(item["Name"].get<string>());
    m_iAnimatorKey = item["AnimatorID"].get<int>();

    for (const auto& strategy : item["Strategies"])
    {
        string strStrategy = strategy.get<string>();
        if (strStrategy == "trace")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::TRACE);
        else if (strStrategy == "random")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::RANDOM_MOVE);
        else if (strStrategy == "heal")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::SELF_HEAL);
        else if (strStrategy == "teleport")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::RANDOM_TELEPORT);
        else if (strStrategy == "barrier")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::SELF_BARRIER);
        else if (strStrategy == "friendly_heal")
            m_vecMosterStrategy.push_back(MONSTER_STRATEGY::FRIENDLY_HEAL);
    }

    m_fHP = item["HP"].get<float>();
    m_fAtt = item["ATT"].get<float>();
    m_iMove = item["Move"].get<int>();
    m_iRange = item["Range"].get<int>();
}