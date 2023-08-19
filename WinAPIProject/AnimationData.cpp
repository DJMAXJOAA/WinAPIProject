#include "pch.h"
#include "AnimationData.h"

AnimationData::AnimationData(int _key)
    : CData(_key)
{
    m_strAnimationName = { "asdf" };
    m_strTexturePath = { "asdfasdf" };
    m_AniInfo = { {1,2},{2,3} };

}

AnimationData::~AnimationData()
{
}

void AnimationData::SaveData()
{
    json j;

    // ���� ����
    ifstream inFile("100.json");
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
            item["Name"] = m_strAnimationName;
            item["Path"] = m_strTexturePath;
            item["AnimationInfo"] = m_AniInfo;  // �ش� Ŭ������ ���� serialize �Լ��� �ʿ��մϴ�.
            updated = true;
            break;  // ��ġ�ϴ� �׸��� ã�����Ƿ� �ݺ��� ����
        }
    }

    // JSON ���Ͽ� �ٽ� ����
    if (updated)
    {
        ofstream outFile("100.json");
        outFile << j.dump(4);  // JSON �����͸� ���� ���� �������Ϸ��� '4'�� ���ڷ� �����մϴ�.
        outFile.close();
    }
}

void AnimationData::LoadData()
{
    json j;

    ifstream inFile("100.json");
    if (!inFile.is_open())
    {
        assert(0);
    }
    inFile >> j;
    inFile.close();

    int targetID = GetKey();
    for (auto& item : j)
    {
        if (item["ID"].get<int>() == targetID)
        {
            // ���ϴ� ID ���� ���� ��ü���� ���� ��������
            m_strAnimationName = item["Name"].get<std::string>();
            m_strTexturePath = item["Path"].get<std::string>();
            m_AniInfo = item["AnimationInfo"].get<AnimationInfo>();
            break;
        }
    }

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
