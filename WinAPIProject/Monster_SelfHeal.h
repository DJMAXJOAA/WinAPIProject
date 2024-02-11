#pragma once
#include "CMonsterStrategy.h"

class Monster_SelfHeal :
    public IMonsterStrategy
{
public:
    virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) override;
};

