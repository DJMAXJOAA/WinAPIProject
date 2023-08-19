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

    // 파일 열기
    ifstream inFile("100.json");
    if (!inFile.is_open())
    {
        assert(0);
    }

    // JSON 파일 읽기
    inFile >> j;
    inFile.close();

    // 원하는 ID 값을 가진 객체 찾기
    int targetID = GetKey();
    bool updated = false;
    for (auto& item : j)
    {
        if (item["ID"].get<int>() == targetID)
        {
            // 원하는 ID 값을 가진 객체의 정보 업데이트
            item["Name"] = m_strAnimationName;
            item["Path"] = m_strTexturePath;
            item["AnimationInfo"] = m_AniInfo;  // 해당 클래스에 대한 serialize 함수가 필요합니다.
            updated = true;
            break;  // 일치하는 항목을 찾았으므로 반복문 종료
        }
    }

    // JSON 파일에 다시 쓰기
    if (updated)
    {
        ofstream outFile("100.json");
        outFile << j.dump(4);  // JSON 데이터를 보기 좋게 포맷팅하려면 '4'를 인자로 전달합니다.
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
            // 원하는 ID 값을 가진 객체에서 정보 가져오기
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
