#include "pch.h"
#include "AnimationData.h"

#include "CPathMgr.h"

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

    // ���� ����
    ifstream inFile(strFilePath);
    if (!inFile.is_open())
    {
        assert(0);
    }

    // JSON ���� �б�
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
            item["AnimationInfo"] = m_AniInfo;  // �ش� Ŭ������ ���� serialize �Լ��� �ʿ��մϴ�.
            updated = true;
            break;  // ��ġ�ϴ� �׸��� ã�����Ƿ� �ݺ��� ����
        }
    }

    // JSON ���Ͽ� �ٽ� ����
    if (updated)
    {
        ofstream outFile(strFilePath);
        outFile << j.dump(4);  // JSON �����͸� ���� ���� �������Ϸ��� '4'�� ���ڷ� �����մϴ�.
        outFile.close();
    }

    DEBUG1(targetID);
}

void AnimationData::ParseData(const json& item)
{
    m_strAnimationName = string_to_wstring(item["Name"].get<std::string>());
    m_strTexturePath = string_to_wstring(item["Path"].get<std::string>());
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
        {"Frame", a.iFrame}
    };
}

void from_json(const json& j, AnimationInfo& a)
{
    a.vLeftTop.x = j.at("Pos").at(0).get<float>();
    a.vLeftTop.y = j.at("Pos").at(1).get<float>();

    a.vSliceSize.x = j.at("Slice").at(0).get<float>();
    a.vSliceSize.y = j.at("Slice").at(1).get<float>();

    a.vStep.x = j.at("Step").at(0).get<float>();
    a.vStep.y = j.at("Step").at(1).get<float>();

    a.vOffset.x = j.at("Offset").at(0).get<float>();
    a.vOffset.y = j.at("Offset").at(1).get<float>();

    a.fDuration = j.at("Duration").get<float>();
    a.iFrame = j.at("Frame").get<int>();
}
