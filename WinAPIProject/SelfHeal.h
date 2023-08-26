#pragma once
#include "IStrategy.h"
class SelfHeal :
    public IMonsterStrategy
{
    CLONE(SelfHeal)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};

