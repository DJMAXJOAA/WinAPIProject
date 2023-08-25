#pragma once
#include "CMonsterFactory.h"

class CMonsterFactory_Stage1 :
    public CMonsterFactory
{
public:
    CMonsterFactory_Stage1();
    ~CMonsterFactory_Stage1();

public:
    virtual void SpawnMonster(map<wstring, CMonster*>& enemyDatas) override;
};

