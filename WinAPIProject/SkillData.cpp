#include "pch.h"
#include "SkillData.h"

SkillData::SkillData(int _key)
	: CData(_key)
{
	LoadData(L"data\\400.json");
}

SkillData::~SkillData()
{
}

void SkillData::ParseData(const json& item)
{
}