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

    // 파일 열기
    ifstream inFile(strFilePath);
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
            item["Name"] = wstring_to_string(m_strAnimationName);
            item["Path"] = wstring_to_string(m_strTexturePath);
            item["AnimationInfo"] = m_AniInfo;  // 해당 클래스에 대한 serialize 함수가 필요합니다.
            updated = true;
            break;  // 일치하는 항목을 찾았으므로 반복문 종료
        }
    }

    // JSON 파일에 다시 쓰기
    if (updated)
    {
        ofstream outFile(strFilePath);
        outFile << j.dump(4);  // JSON 데이터를 보기 좋게 포맷팅하려면 '4'를 인자로 전달합니다.
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
