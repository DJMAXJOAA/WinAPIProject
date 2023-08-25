#include "pch.h"
#include "CMonsterFactory_Stage2.h"

#include "CMonster.h"

CMonsterFactory_Stage2::CMonsterFactory_Stage2()
{
}

CMonsterFactory_Stage2::~CMonsterFactory_Stage2()
{
}

void CMonsterFactory_Stage2::SpawnMonster(map<wstring, CMonster*>& enemyDatas)
{
	for (int i = 0; i < 2; i++)
	{
		CMonster* pRabbit = enemyDatas[L"Rabbit"]->Clone();
		CreateObj(pRabbit, GROUP_TYPE::MONSTER);

		CMonster* pMushroom = enemyDatas[L"Mushroom"]->Clone();
		CreateObj(pMushroom, GROUP_TYPE::MONSTER);
	}
}
