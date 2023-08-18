#include "pch.h"
#include "AnimationData.h"

AnimationData::AnimationData(int _key)
	: CData(_key)
{
}

AnimationData::~AnimationData()
{
}

void AnimationData::LoadData()
{
    json j;

    ifstream inFile("animation.json");
    if (!inFile.is_open())
    {
        assert(0);
    }
    inFile >> j;
    inFile.close();
    
    m_strTextureName.assign(j["Name"].begin(), j["Name"].end());
    m_strTexturePath.assign(j["Path"].begin(), j["Path"].end());
}

void to_json(json& j, const AnimationInfo& p)
{

}

void from_json(const json& j, AnimationInfo& p)
{
}
