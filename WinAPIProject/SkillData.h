#pragma once
#include "CData.h"

struct SkillInfo
{
	TILE_STATE	tileColor;		// ��ų Ÿ�� ����
	float		fValueRatio;	// ��ų ���
	int			iSkillRange;	// ��ų ��Ÿ�
	int			iSkillCooldown;	// ��ų ��Ÿ��
};

void from_json(const json& j, SkillInfo& a);

class SkillData :
	public CData
{
private:
	wstring		m_strSkillName;			// ��ų �̸�
	wstring		m_strSkillExplanation;	// ��ų ����
	wstring		m_strIconPath;			// ������ �ؽ��� ��ġ
	SkillInfo	m_SkillInfo;			// ��ų ���� ����ü
	GRADE		m_skillGrade;		// ��ų ���

public:
	SkillData(int _key);
	~SkillData();

public:
	virtual void ParseData(const json& item) override;
};

