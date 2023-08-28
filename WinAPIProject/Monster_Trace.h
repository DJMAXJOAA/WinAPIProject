#pragma once
#include "CMonsterStrategy.h"

class Monster_Trace :
    public CMonsterStrategy
{
public:
    virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) override;
};

