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

    m_iFrame = j["Frame"];
    m_vLeftTop.x = j["XPos"];
    m_vLeftTop.y = j["YPos"];
    m_vSliceSize.x = j["Width"];
    m_vSliceSize.y = j["Height"];
    m_vOffset.x = j["Xoffset"];
    m_vOffset.y = j["Yoffset"];
}
