#include "pch.h"
#include "AnimatorData.h"

#include "CDataUtils.h"

AnimatorData::AnimatorData(int _key)
	: CData(_key)
{
    LoadData(L"data\\200.json");
}

AnimatorData::~AnimatorData()
{
}

void AnimatorData::ParseData(const json& item)
{
    m_strAnimatorName = string_to_wstring(item["Name"].get<string>());
    m_vecAnimation = item["AnimationList"].get<vector<int>>();
}
