#pragma once
#include "CMonsterStrategy.h"

class Monster_Teleport :
    public CMonsterStrategy
{
public:
    virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) override;
};

