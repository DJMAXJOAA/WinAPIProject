#pragma once
#include "CData.h"

class SkillData :
	public CData
{
private:

public:
	SkillData(int _key);
	~SkillData();

public:
	virtual void LoadData() override;
};

