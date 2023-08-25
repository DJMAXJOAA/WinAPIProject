#pragma once
#include "CMonsterFactory.h"

class CMonsterFactory_Stage2 :
    public CMonsterFactory
{
public:
    CMonsterFactory_Stage2();
    ~CMonsterFactory_Stage2();

public:
    virtual void SpawnMonster(map<wstring, CMonster*>& enemyDatas) override;
};

