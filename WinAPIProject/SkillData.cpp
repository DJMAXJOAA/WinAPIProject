#include "pch.h"
#include "SkillData.h"

#include "CDataUtils.h"

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
	m_strSkillName = string_to_wstring(item["Name"].get<string>());
	m_strSkillExplanation = string_to_wstring(item["Explanation"].get<string>());
	m_strIconPath = string_to_wstring(item["Path"].get<string>());
	m_SkillInfo = item["SkillInfo"].get<SkillInfo>();
	string skillGrade = item["Grade"].get<string>();
	if (skillGrade == "common")
		m_skillGrade = GRADE::COMMON;
	else if (skillGrade == "rare")
		m_skillGrade = GRADE::RARE;
	else if (skillGrade == "epic")
		m_skillGrade = GRADE::EPIC;
	else if (skillGrade == "unique")
		m_skillGrade = GRADE::UNIQUE;
}

void from_json(const json& j, SkillInfo& a)
{
	string skillColor = j["TileColor"].get<string>();
	if (skillColor == "red")
		a.tileColor = TILE_STATE::RED;
	else if (skillColor == "green")
		a.tileColor = TILE_STATE::GREEN;
	else if (skillColor == "blue")
		a.tileColor = TILE_STATE::BLUE;
	else if (skillColor == "purple")
		a.tileColor = TILE_STATE::PURPLE;

	a.fValueRatio = j["Ratio"].get<float>();
	a.iSkillRange = j["Range"].get<int>();
	a.iSkillCooldown = j["Cooldown"].get<int>();
}
