#include "pch.h"
#include "CMonsterFactory_Stage1.h"

#include "CMonster.h"

CMonsterFactory_Stage1::CMonsterFactory_Stage1()
{
}

CMonsterFactory_Stage1::~CMonsterFactory_Stage1()
{
}

void CMonsterFactory_Stage1::SpawnMonster(map<wstring, CMonster*>& enemyDatas)
{
	for (int i = 0; i < 4; i++)
	{
		CMonster* pRabbit = enemyDatas[L"rabbit"]->Clone();
		CreateObj(pRabbit, GROUP_TYPE::MONSTER);
	}
}
