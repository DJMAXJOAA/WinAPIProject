#pragma once
#include "CData.h"

struct SkillInfo
{
	TILE_STATE	tileColor;		// 스킬 타일 색깔
	float		fValueRatio;	// 스킬 계수
	int			iSkillRange;	// 스킬 사거리
	int			iSkillCooldown;	// 스킬 쿨타임
};

void from_json(const json& j, SkillInfo& a);

class SkillData :
	public CData
{
private:
	wstring		m_strSkillName;			// 스킬 이름
	wstring		m_strSkillExplanation;	// 스킬 설명
	wstring		m_strIconPath;			// 아이콘 텍스쳐 위치
	SkillInfo	m_SkillInfo;			// 스킬 정보 구조체
	GRADE		m_skillGrade;		// 스킬 등급

public:
	SkillData(int _key);
	~SkillData();

public:
	virtual void ParseData(const json& item) override;
};

