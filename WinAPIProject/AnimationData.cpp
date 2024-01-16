#include "pch.h"
#include "AnimationData.h"

#include "CPathMgr.h"
#include "CDataUtils.h"

AnimationData::AnimationData(int _key)
    : CData(_key)
{
    LoadData(L"data\\100.json");
}

AnimationData::~AnimationData()
{
}

void AnimationData::SaveData()
{
    json j;

    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += L"data\\100.json";

    // json ���� ����
    ifstream inFile(strFilePath);
    if (!inFile.is_open())
    {
        assert(0);
    }

    // json ���� �б�
    inFile >> j;
    inFile.close();

    // ���ϴ� ID ���� ���� ��ü ã��
    int targetID = GetKey();
    bool updated = false;
    for (auto& item : j)
    {
        if (item["ID"].get<int>() == targetID)
        {
            // ���ϴ� ID ���� ���� ��ü�� ���� ������Ʈ
            item["Name"] = wstring_to_string(m_strAnimationName);
            item["Path"] = wstring_to_string(m_strTexturePath);
            item["AnimationInfo"] = m_AniInfo;
            updated = true;
            break;  // ��ġ�ϴ� �׸��� ã�����Ƿ� �ݺ��� ����
        }
    }

    // JSON ���Ͽ� �ٽ� ����
    if (updated)
    {
        ofstream outFile(strFilePath);
        outFile << j.dump(4);
        outFile.close();
    }

    DEBUG1(targetID);
}

void AnimationData::ParseData(const json& item)
{
    m_strAnimationName = string_to_wstring(item["Name"].get<string>());
    m_strTexturePath = string_to_wstring(item["Path"].get<string>());
    m_strAnimationType = string_to_wstring(item["Type"].get<string>());
    m_AniInfo = item["AnimationInfo"].get<AnimationInfo>();
}

void to_json(json& j, const AnimationInfo& a)
{
    j = json
    {
        {"Pos", vec2_to_json(a.vLeftTop)},
        {"Slice", vec2_to_json(a.vSliceSize)},
        {"Step", vec2_to_json(a.vStep)},
        {"Offset", vec2_to_json(a.vOffset)},
        {"Duration", a.fDuration},
        {"Frame", a.iFrame},
        {"EventFrame", a.iEventFrame}
    };
}

void from_json(const json& j, AnimationInfo& a)
{
    a.vLeftTop.x = j["Pos"][0].get<float>();
    a.vLeftTop.y = j["Pos"][1].get<float>();

    a.vSliceSize.x = j["Slice"][0].get<float>();
    a.vSliceSize.y = j["Slice"][1].get<float>();

    a.vStep.x = j["Step"][0].get<float>();
    a.vStep.y = j["Step"][1].get<float>();

    a.vOffset.x = j["Offset"][0].get<float>();
    a.vOffset.y = j["Offset"][1].get<float>();

    a.fDuration = j["Duration"].get<float>();
    a.iFrame = j["Frame"].get<int>();
    a.iEventFrame = j["EventFrame"].get<int>();
}
